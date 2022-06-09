#include "common.h"
#include <iostream>
#define PORT_NUM    10200
#define MAX_MSG_LEN 256

int main()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2,2), &wsadata); //윈속 초기화
    char server_ip[40] = "";
    printf("서버 IP:");
    std::cin.getline(server_ip, sizeof(server_ip));
    SOCKET sock;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //소켓 생성
    SOCKADDR_IN servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server_ip);
    servaddr.sin_port = htons(PORT_NUM);
    int re;
    re = connect(sock, (SOCKADDR*)&servaddr, sizeof(servaddr));
    if(re == -1){return -1;}

    char msg[MAX_MSG_LEN]="";
    while(true)
    {
        std::cin.getline(msg, MAX_MSG_LEN);
        send(sock, msg, sizeof(msg), 0);
        if(strcmp(msg, "exit")==0)
        {
            break;
        }
        recv(sock, msg, sizeof(msg), 0);
        printf("수신: %s\n", msg);
    }
    closesocket(sock);
    WSACleanup();
}