#pragma comment (lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <WinSock2.h>

using namespace std;
void err_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}
void main() {
  WSADATA wsaData;
  SOCKET servSock;
  SOCKADDR_IN servAddr, clntAddr;
  char message[BUFSIZ];

  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    err_handling("WSAstartup err");
  }

  servSock = socket(PF_INET, SOCK_DGRAM, 0);
  if (servSock == INVALID_SOCKET) {
    err_handling("servsock err");
  }

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  // or htonl(INADDR_ANY)
  servAddr.sin_port = htons(9000);

  if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
    err_handling("bind err");
  }
  // TCP처럼 listen, accept가 없음
  while (1) {
    int clntsize = sizeof(clntAddr);
    recvfrom(servSock, message, BUFSIZ, 0, (SOCKADDR*)&clntAddr, &clntsize); // TCP와 달리 recvfrom
    sendto(servSock, message, BUFSIZ, 0, (SOCKADDR*)&clntAddr, clntsize); // TCP와 달리 sendtos
  }
  closesocket(servSock);
  WSACleanup();
}