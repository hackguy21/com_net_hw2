#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE    100
#define IP_ADDR     "127.0.0.1"


int main() {

    // 'https://learn.microsoft.com/ko-kr/windows/win32/winsock/creating-a-socket-for-the-server' 참조
    // 소켓
    WSADATA wsadata;
    SOCKET sock;
    SOCKADDR_IN servAdr;
    char message[BUF_SIZE];
    int strLen;

    // 윈도우 운영체제에서 소켓dll 생성 실패 예외처리
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        fputs("wsastartup() error\n", stderr);
        exit(1);
    }

    // 소켓 설정, SOCK_DGRAM으로 안하여 UDP로 설정됨.
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        fputs("upd 소켓 생성 실패\n", stderr);
        exit(1);
    }

    // 소켓에 들어갈 주소정보 설정

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET; // IPv4주소체계 사용
    servAdr.sin_addr.s_addr = inet_addr(IP_ADDR); // 127.0.0.1로 루프백 주소 사용
    servAdr.sin_port = htons(1234); // 임의로 1234번 포트 사용

    // 서버와 연결
    connect(sock, (SOCKADDR*)&servAdr, sizeof(servAdr));

    // 패킷송수신 과정
    while (1) {
        fputs("command(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if (!strcmp(message, "q\n")) {
            break;
        }

        // sock : 통신에 사용할 소켓파일, message : 보내는 메세지 내용 및 크기
        send(sock, message, strlen(message), 0);


        strLen = recv(sock, message, sizeof(message) - 1, 0);
        // 서버에서 보내온 문자열 데이터에 null 문자 삽입
        message[strLen] = '\0';
        printf("%s\n", message);
    }

    // 소켓 종료
    // 윈도우 운영체제 상에서 소켓dll 초기화
    closesocket(sock);
    WSACleanup();

    return 0;
}
