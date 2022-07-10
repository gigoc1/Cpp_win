/*
  - https://m.blog.naver.com/beodeulpiri/221461025247
  - openssl req -x509 -nodes -days 365 -newkey rsa:1024 -keyout mycert.pem -out mycert.pem
  - tasks.json
                        "-g",
                        "${fileDirname}\\${fileBasenameNoExtension}.cpp",
                        "-o",
                        "${fileDirname}\\${fileBasenameNoExtension}.exe",
                        "-lws2_32",
                        "-IC:/Program Files/OpenSSL-Win64/include",
                        "-LC:/Program Files/OpenSSL-Win64/lib",
                        "-llibssl",
                        "-llibcrypto"
*/

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "libeay32.lib")

#include <windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <strsafe.h>

#define IP_ADDR localhost
#define DEFAULT_PORT "9002"

#define FAIL -1

void DebugMsg(const CHAR *pszFormat, ...)
{
#ifdef _DEBUG
    CHAR buf[1024];
    StringCchPrintf(buf, sizeof(buf) / sizeof(CHAR), "(%lu): ", GetCurrentThreadId());
    va_list arglist;
    va_start(arglist, pszFormat);
    StringCchVPrintf(&buf[strlen(buf)], sizeof(buf) / sizeof(CHAR), pszFormat, arglist);
    va_end(arglist);
    StringCchCat(buf, sizeof(buf) / sizeof(CHAR), "\n");
    printf(buf);
#endif
}

void PrintHexDump(DWORD length, const void *const buf, const bool verbose = false)
{
    DWORD i, count, index;
    CHAR rgbDigits[] = "0123456789abcdef";
    CHAR rgbLine[100];
    char cbLine;
    const unsigned char *buffer = static_cast<const unsigned char *>(buf);

    if (!verbose & (length > 16))
        length = 16;

    for (index = 0; length;
         length -= count, buffer += count, index += count)
    {
        count = (length > 16) ? 16 : length;

        sprintf_s(rgbLine, 100, "%4.4x  ", index);
        cbLine = 6;

        for (i = 0; i < count; i++)
        {
            rgbLine[cbLine++] = rgbDigits[buffer[i] >> 4];
            rgbLine[cbLine++] = rgbDigits[buffer[i] & 0x0f];
            if (i == 7)
            {
                rgbLine[cbLine++] = ':';
            }
            else
            {
                rgbLine[cbLine++] = ' ';
            }
        }
        for (; i < 16; i++)
        {
            rgbLine[cbLine++] = ' ';
            rgbLine[cbLine++] = ' ';
            rgbLine[cbLine++] = ' ';
        }

        rgbLine[cbLine++] = ' ';

        for (i = 0; i < count; i++)
        {
            if (buffer[i] < 32 || buffer[i] > 126)
            {
                rgbLine[cbLine++] = '.';
            }
            else
            {
                rgbLine[cbLine++] = buffer[i];
            }
        }

        rgbLine[cbLine++] = 0;
        DebugMsg(rgbLine);
    }
}

SSL_CTX *InitServerCTX(void)
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms(); /* load & register all cryptos, etc. */
    SSL_load_error_strings();     /* load all error messages */

    method = TLSv1_2_server_method();
    ctx = SSL_CTX_new(method); /* create new context from method */
    if (ctx == NULL)
    {
        printf("SSL_CTX_new error\n");
        ERR_print_errors_fp(stderr);
        abort();
    }

    SSL_CTX_set_cipher_list(ctx, "ALL:eNULL");

    return ctx;
}

void LoadCertificates(SSL_CTX *ctx, char *CertFile, char *KeyFile)
{
    // New lines
    if (SSL_CTX_load_verify_locations(ctx, CertFile, KeyFile) != 1)
    {
        printf("SSL_CTX_load_verify_locations error\n");
        ERR_print_errors_fp(stderr);
    }

    if (SSL_CTX_set_default_verify_paths(ctx) != 1)
    {
        printf("SSL_CTX_set_default_verify_paths error\n");
        ERR_print_errors_fp(stderr);
    }
    // End new lines

    /* set the local certificate from CertFile */
    if (SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0)
    {
        printf("SSL_CTX_use_certificate_file error\n");
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    char* str="12345678";
    SSL_CTX_set_default_passwd_cb_userdata(ctx, str);
    if (SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0)
    {
        printf("SSL_CTX_use_PrivateKey_file error\n");
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if (!SSL_CTX_check_private_key(ctx))
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }

    // New lines - Force the client-side have a certificate
    // SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
    // SSL_CTX_set_verify_depth(ctx, 4);
    // End new lines
}

void ShowCerts(SSL *ssl)
{
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
    if (cert != NULL)
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
        printf("No certificates.\n");
}
/**
 * Example SSL server that accepts a client and echos back anything it receives.
 * Test using `curl -I https://127.0.0.1:8081 --insecure`
 */
int main(int arc, char **argv)
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;
    sockaddr_in sock_serv;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // 아래 코드는 가용 ip를 모두 불러오므로 필요한 ip만 적용하기로 함
    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    
    sock_serv.sin_family = AF_INET;
    sock_serv.sin_addr.s_addr = inet_addr("172.19.224.1");
    sock_serv.sin_port = htons(9002);

    printf("server ip is %s\n", inet_ntoa(sock_serv.sin_addr));

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, (struct sockaddr*)&sock_serv, sizeof(sock_serv));
    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    printf("Setup the TCP listening socket: port[%s]\n", DEFAULT_PORT);
    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET)
    {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    printf("Accept a client socket.\n");
    // No longer need server socket
    closesocket(ListenSocket);

    char CertFile[] = "mycert.pem";
    char KeyFile[] = "mycert.pem";

    SSL_library_init();

    SSL_CTX *ctx = InitServerCTX();           /* initialize SSL */
    LoadCertificates(ctx, CertFile, KeyFile); /* load certs */
    printf("LoadCertificates.\n");

    SSL *ssl = SSL_new(ctx);
    printf("SSL_new.\n");
    BIO *accept_bio = BIO_new_socket(ClientSocket, BIO_CLOSE);
    SSL_set_bio(ssl, accept_bio, accept_bio);

    SSL_accept(ssl);
    printf("SSL_accept.\n");

    BIO *bio = BIO_pop(accept_bio);

    char buf[1024];
    memset(buf, 0x00, 1024);
    while (1)
    {
        // first read data
        int r = SSL_read(ssl, buf, 1024);
        printf("SSL_read buf len:%d\n", r);
        PrintHexDump(r, buf);
        switch (SSL_get_error(ssl, r))
        {
        case SSL_ERROR_NONE:
            break;
        case SSL_ERROR_ZERO_RETURN:
            goto end;
        default:
            printf("SSL read problem");
            goto end;
        }

        int len = r;

        // now keep writing until we've written everything
        int offset = 0;
        while (len)
        {
            r = SSL_write(ssl, buf + offset, len);
            switch (SSL_get_error(ssl, r))
            {
            case SSL_ERROR_NONE:
                len -= r;
                offset += r;
                break;
            default:
                printf("SSL write problem");
                goto end;
            }
        }
    }

end:

    SSL_shutdown(ssl);

    BIO_free_all(bio);
    BIO_free_all(accept_bio);

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}