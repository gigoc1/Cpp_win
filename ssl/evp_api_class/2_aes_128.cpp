#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/evp.h>

// 읽어 들이는 바이트 수
#define NUM_BYTES_READ 32
int do_crypt(const unsigned char *key, const unsigned char *iv, FILE *ifp, FILE *ofp, int isEncrypt);
int main(int argc, const char *argv[])
{
    // EVP API를 써서 aes_cbc를 하기위해 초가화 과정
    EVP_CIPHER_CTX *ctx_enc = EVP_CIPHER_CTX_new();
    if (ctx_enc == NULL)
    {
        printf("EVP_CIPHER_CTX_new is NULL error!!\n");
        return -1;
    }

    if (!(EVP_CipherInit_ex(ctx_enc, EVP_aes_128_cbc(), NULL, NULL, NULL, 1)))
    {
        printf("EVP_CipherInit_ex error!!\n");
        return -1;
    }

    /**************************************************************************************************
     key, iv initialized
     Random key, iv use RAND_bytes()
     **************************************************************************************************/

    printf("\n");
    printf("****************************Key, IV initailized********************************\n");
    printf("\n");
    // key와 iv값 동적할당공간 생성
    unsigned char *key = (unsigned char *)malloc(EVP_CIPHER_CTX_key_length(ctx_enc)); // key
    unsigned char *iv = (unsigned char *)malloc(EVP_CIPHER_CTX_iv_length(ctx_enc));   // iv

    if (!RAND_bytes(key, EVP_CIPHER_CTX_key_length(ctx_enc)))
    {
        printf("RAND_bytes function error!!\n");
        return -1;
    }
    if (!RAND_bytes(iv, EVP_CIPHER_CTX_iv_length(ctx_enc)))
    {
        printf("RAND_bytes function error!!\n");
        return -1;
    }

    // key 와 iv 값 확인!!
    printf("Use key value : ");
    for (int i = 0; i < (int)strlen((const char *)key); i++) // key값 출력
        printf("0x%02X ", key[i]);
    printf("\n");
    printf("Use iv value : ");
    for (int i = 0; i < (int)strlen((const char *)iv); i++) // iv값 출력
        printf("0x%02X ", iv[i]);
    printf("\n");

    /*
     *
     * Enctypt
     *
     *
     */
    printf("\n");
    printf("******************************Encrypyion**********************************\n");
    printf("\n");
    // FILE입출력을 통한 암호화를 진행하기 위해 경로를 설정해준다!!
    // input 파일은 기존에 미리 있어야한다!! 나와 같이 경로를 꼭 설정 해줘야하는것이 아닌 그냥 파일명만 써도되는데 그건 프로젝트 디렉토리일떄만 가능하다!!!!
    FILE *ifp = fopen("plain.txt", "r");
    // 이건 출력파일인데 이것은 없으면 새로 생성되고, 있어도 새로 덮어씌워진다.
    FILE *ofp = fopen("cypher.txt", "w");
    // fopen error check
    if (ifp == NULL)
    {
        printf("file open error!!\n");
        return -1;
    }
    if (ofp == NULL)
    {
        printf("file open error!!\n");
        return -1;
    }

    do_crypt(key, iv, ifp, ofp, 1);
    fclose(ifp);
    fclose(ofp);
    /*
     *
     * Decryption
     *
     */

    // FILE입출력을 통한 암호화를 진행하기 위해 경로를 설정해준다!!
    // input 파일은 기존에 미리 있어야한다!! 나와 같이 경로를 꼭 설정 해줘야하는것이 아닌 그냥 파일명만 써도되는데 그건 프로젝트 디렉토리일떄만 가능하다!!!!

    ifp = fopen("cypher.txt", "r");

    // 이건 출력파일인데 이것은 없으면 새로 생성되고, 있어도 새로 덮어씌워진다.

    ofp = fopen("decryption.txt", "w");

    // fopen error check

    if (ifp == NULL)
    {

        printf("file open error!!\n");

        return -1;
    }

    if (ofp == NULL)
    {

        printf("file open error!!\n");

        return -1;
    }

    do_crypt(key, iv, ifp, ofp, 0);

    // 각각 할당을 해제시켜준다.

    fclose(ifp);

    fclose(ofp);

    return 0;
}

int do_crypt(const unsigned char *key, const unsigned char *iv, FILE *ifp, FILE *ofp, int isEncrypt)
{

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    if (ctx == NULL)
    {

        printf("EVP_CIPHER_CTX_new is NULL error!!\n");

        return -1;
    }

    if (!(EVP_CipherInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv, isEncrypt)))
    {

        printf("EVP_CipherInit_ex error!!\n");

        return -1;
    }

    unsigned char *input_buf = (unsigned char *)malloc(NUM_BYTES_READ);

    unsigned char *output_buf = (unsigned char *)malloc(NUM_BYTES_READ + EVP_CIPHER_CTX_block_size(ctx));

    if (input_buf == NULL)
    {

        printf("cipher_buf_dec allocaion error!! \n");

        return -1;
    }

    if (output_buf == NULL)
    {

        printf("plain_buf_dec allocaion error!! \n");

        return -1;
    }

    int inlen = 0, outtxtlen = 0;

    while (1)
    {

        // 여기서 fread는 ifp에서 1바이트씩 읽어 들여와 총 32바이트를  plain_buf에 저장하는 역할을 한다.

        // 만약 성공적으로 지정된 바이트 수만큼 읽어 들였다면 읽어들인 원소의 개수가 size_t 자료형으로 반환 된다.

        // 아니면 0을 반환한다.

        inlen = (int)fread(input_buf, 1, NUM_BYTES_READ, ifp);

        // 정해진 바이트 수에 충족하지 못하면 Final함수를 호출하기위해 반복문을 빠져나간다. 여기선 NUM_BYTES_READ

        if (inlen <= 0)

            break;

        // EVP_CipherUpdate(<#EVP_CIPHER_CTX *ctx#>, <#unsigned char *out#>, <#int *outl#>, <#const unsigned char *in#>, <#int inl#>)

        // ctx_enc를 참조해서 plain_buf에서 inlength만큼 cipher_buf에 암호문을 쓴다 그리고 암호화된 바이트 수만큼 ci_txtlength로 반환된다.

        if (1 != EVP_CipherUpdate(ctx, output_buf, &outtxtlen, input_buf, inlen))
        {

            printf("EVP_CipherUpdate error!!! \n");

            return -1;
        }

        printf("\n inlen = %d , ctxlen = %d", inlen, outtxtlen);

        printf("\n\n");

        // 암호문 출력

        fwrite(output_buf, 1, outtxtlen, ofp);
    }

    // ctx_enc는 cipher_buf가 무슨파일이 어디까지 암호화 되있는지 알고있다 따라서 cipher_buf에 ci_txtlength만 인자로 넘겨주면 된다.

    // EVP_CipherFinal(<#EVP_CIPHER_CTX *ctx#>, <#unsigned char *outm#>, <#int *outl#>)

    EVP_CipherFinal(ctx, output_buf, &outtxtlen);

    fwrite(output_buf, 1, outtxtlen, ofp);

    printf("\n ctxlen = %d \n\n", outtxtlen);

    EVP_CIPHER_CTX_free(ctx);

    free(input_buf);

    free(output_buf);

    return 0;
}