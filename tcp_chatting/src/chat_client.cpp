#include "common.h"
#include <iostream>
#define PORT_NUM    10200
#define MAX_MSG_LEN 256
#define SERVER_IP   "192.168.137.1"

void RecvThreadPoint(void* pin);
int main()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2,2), &wsadata);
    SOCKET sock;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == -1) {return -1;}
    SOCKADDR_IN servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servaddr.sin_port = htons(PORT_NUM);

    int re =0;
    re = connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(re == -1)
    {
        return -1;
    }
    _beginthread(RecvThreadPoint, 0, (void*)sock);
    
    char msg[MAX_MSG_LEN]= "";
    while(true)
    {
        std::cin.getline(msg, MAX_MSG_LEN);
        send(sock, msg, sizeof(msg), 0);
        if(strcmp(msg, "exit") ==0)
        {
            break;
        }
    }
    closesocket(sock);
    WSACleanup();
    return 0;
}

void RecvThreadPoint(void* pin)
{
    SOCKET sock = (SOCKET)pin;
    char msg[MAX_MSG_LEN];
    while(recv(sock, msg, MAX_MSG_LEN, 0))
    {
        printf("%s\n", msg);
    }
    closesocket(sock);
}