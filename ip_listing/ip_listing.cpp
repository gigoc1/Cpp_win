//���� ȣ��Ʈ IPv4 �ּ� ������
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
// #pragma warning(disable:4996)

#pragma comment(lib,"ws2_32")
void ViewLocalHostIPv4();
int main()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);    

    ViewLocalHostIPv4();

    WSACleanup();
    return 0;
}

void ViewLocalHostIPv4()
{
    char localhostname[MAX_PATH];
    IN_ADDR addr = { 0, };

    if (gethostname(localhostname, MAX_PATH) == SOCKET_ERROR)//ȣ��Ʈ �̸� ������
    {
        return;
    }
    printf("localhostname: %s\n", localhostname);
    HOSTENT* ptr = gethostbyname(localhostname);//ȣ��Ʈ ��Ʈ�� ������
    while (ptr && ptr->h_name)
    {
        if (ptr->h_addrtype == PF_INET)//IPv4 �ּ� Ÿ���� ��
        {
            for (int index = 0; ptr->h_addr_list[index]; index++)
            {
                memcpy(&addr, ptr->h_addr_list[index], ptr->h_length);//�޸� ����
                printf("%s, ���ڿ�: %s\n", inet_ntoa(addr), ptr->h_addr_list[index]);
            }
        }
        ptr++;
    }
}