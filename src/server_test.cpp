#include "common.h"
#define PORT_NUM    10200
#define BLOG_SIZE   5
#define MAX_MSG_LEN 256

SOCKET SetTCPServer(short pnum, int blog)
{
    SOCKET sock;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //���� ����
    SOCKADDR_IN servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr = GetDefaultMyIP();
    servaddr.sin_port = htons(pnum);
    int re = 0;
    re = bind(sock, (SOCKADDR*)&servaddr, sizeof(servaddr));
    if(re == -1){return -1;}
    re = listen(sock, blog); //��α� ť ũ�� ����
    if(re == -1){return -1;}
    printf("%s:%d Setup\n", inet_ntoa(servaddr.sin_addr), pnum);
    return sock;
}

void DoIt(void * param)
{
    SOCKET dosock = (SOCKET)param;
    SOCKADDR_IN cliaddr = {0};
    int len = sizeof(cliaddr);
    getpeername(dosock, (SOCKADDR*)&cliaddr, &len);
    char msg[MAX_MSG_LEN]="";
    while(recv(dosock, msg, sizeof(msg),0) > 0)
    {
        printf("%s.%d�� ���� recv:%s\n",
        inet_ntoa(cliaddr.sin_addr),
        ntohs(cliaddr.sin_port),
         msg);
        send(dosock, msg, sizeof(msg), 0);
    }
    printf("%s:%d�� ��� ����\n", 
    inet_ntoa(cliaddr.sin_addr),
    ntohs(cliaddr.sin_port));
    closesocket(dosock);
}
void AcceptLoop(SOCKET sock)
{
    SOCKET dosock;
    SOCKADDR_IN cliaddr = {0};
    int len = sizeof(cliaddr);

    while(true)
    {
        dosock = accept(sock, (SOCKADDR*)&cliaddr, &len); //���� ����
        if(dosock == -1)
        {
            perror("accept ����");
            break;
        }
        printf("%s:%d�� ���� ��û ����\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
        // DoIt(dosock);
        _beginthread(DoIt, 0, (void *)dosock);
    }
}

int main()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata); //���� �ʱ�ȭ

    #if 0
    {
    unsigned int idata = 0x123456;
    unsigned short sdata = 0x1234;
    printf("host: %#x network: %#x \n", idata, htonl(idata));
    printf("host: %#x network: %#x \n", sdata, htons(sdata));
    
    IN_ADDR addr;
    addr.s_addr = htonl(12<<24|34<<16|56<<8|78);
    printf("%s\n", inet_ntoa(addr));

    u_int naddr = inet_addr("192.168.34.0");
    u_int haddr = ntohl(naddr);
    printf("host: %#x network: %#x \n", haddr, naddr);
    
    printf("%d.%d.%d.%d\n", haddr>>24, (u_char)(haddr>>16),
    (u_char)(haddr>>8), (u_char)haddr);

    printf("My host ip is: %s", inet_ntoa(GetDefaultMyIP()));
    }
    #endif

    SOCKET sock = SetTCPServer(PORT_NUM, BLOG_SIZE); //��� ���� ����
    AcceptLoop(sock); //Accept Loop
    closesocket(sock); //���� ����


    WSACleanup();
    return 0;
}

