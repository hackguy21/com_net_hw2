
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>			//���� ������� ����
#pragma comment(lib,"ws2_32")	//���� ���̺귯�� ����

#define _CRT_SECURE_NO_WARNINGS
#define BUF_SIZE 30


int main(int argc, char* argv[])
{
	
	WSADATA wsadata; // Windows ���� DLL, �����쿡�� �����ϴ� ����DLL���� ��ü
	SOCKET servSock;
	char message[BUF_SIZE];
	int strLen;
	int clntAdrSz;
	SOCKADDR_IN servAdr, clntAdr;

	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		fputs("wsastartup() error\n", stderr);
		exit(1);
	}


	// 1.socket�� ����

	servSock = socket(PF_INET, SOCK_DGRAM, 0);	
	// PF_INET = IPv4�� ����� �ּ�ü����
	// UDP ���ϻ��� SOCK_DGRAM <-> TCP ���ϻ��� SOCK_STREAM
	if (servSock == INVALID_SOCKET) { // ���� �������� ����ó��
		WSACleanup();
		fputs("upd ���� ���� ����\n", stderr);
		exit(1);
	}



	// �����ʿ� �ּ������� �������
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET; // �ּ�ü��� IPv4�� ���
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY); // ������ǻ���� IP�� ���
	servAdr.sin_port = htons(1234); // ������ ��Ʈ��ȣ�� ���� // ���⼱ ���Ƿ� 1234�� ��Ʈ

	// 2.bind
	// ������ �ּ������� ���ε�. �������ϰ� ����ü�� �Ķ���ͷ� �Է�
	// ���ε� ���� ����ó��
	if (bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) {
		WSACleanup();
		fputs("binding error\n", stderr);
		exit(1);
	}
	// 3. recvfrom(), sendto() �Լ��� ���� ��Ŷ�ۼ���

	printf("Socket ���� ����");

	while (1)
	{
		clntAdrSz = sizeof(clntAdr);
		// �������ϰ�, �����͸� ������ ����, ���ۻ�����, 
		strLen = recvfrom(servSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAdr, &clntAdrSz);
		// recvfrom�� ���� Ŭ���̾�Ʈ�� ���� ���� �����͸� ��ȯ
		sendto(servSock, "hi\n", 4, 0, (SOCKADDR*)&clntAdr, sizeof(clntAdr));
	}

	// 4. socket �ݱ�
	closesocket(servSock); // ���� �����Լ�
	WSACleanup();		//������󿡼� �����ϴ� �����ʱ�ȭ �Լ�
	return 0;
}