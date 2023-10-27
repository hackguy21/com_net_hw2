#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE    100
#define IP_ADDR     "127.0.0.1"


int main() {

    // 'https://learn.microsoft.com/ko-kr/windows/win32/winsock/creating-a-socket-for-the-server' ����
    // ����
    WSADATA wsadata;
    SOCKET sock;
    SOCKADDR_IN servAdr;
    char message[BUF_SIZE];
    int strLen;

    // ������ �ü������ ����dll ���� ���� ����ó��
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        fputs("wsastartup() error\n", stderr);
        exit(1);
    }

    // ���� ����, SOCK_DGRAM���� ���Ͽ� UDP�� ������.
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        fputs("upd ���� ���� ����\n", stderr);
        exit(1);
    }

    // ���Ͽ� �� �ּ����� ����

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET; // IPv4�ּ�ü�� ���
    servAdr.sin_addr.s_addr = inet_addr(IP_ADDR); // 127.0.0.1�� ������ �ּ� ���
    servAdr.sin_port = htons(1234); // ���Ƿ� 1234�� ��Ʈ ���

    // ������ ����
    connect(sock, (SOCKADDR*)&servAdr, sizeof(servAdr));

    // ��Ŷ�ۼ��� ����
    while (1) {
        fputs("command(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if (!strcmp(message, "q\n")) {
            break;
        }

        // sock : ��ſ� ����� ��������, message : ������ �޼��� ���� �� ũ��
        send(sock, message, strlen(message), 0);


        strLen = recv(sock, message, sizeof(message) - 1, 0);
        // �������� ������ ���ڿ� �����Ϳ� null ���� ����
        message[strLen] = '\0';
        printf("%s\n", message);
    }

    // ���� ����
    // ������ �ü�� �󿡼� ����dll �ʱ�ȭ
    closesocket(sock);
    WSACleanup();

    return 0;
}
