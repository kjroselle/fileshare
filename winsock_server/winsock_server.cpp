// winsock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
#define DEFAULT_IP "192.168.1.4"

int main(int argc,char* argv[])
{
  WSADATA wsaData;

  int iResult;

  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (iResult != 0) {
    printf("WSAStartup failed: %d\n", iResult);
    return 1;
  } else {
    printf("WSAStartup success\n");
  }

  // server
  struct addrinfo *result = NULL, *ptr = NULL, hints;

  ZeroMemory( &hints, sizeof(hints) );
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  // Resolve the server address and port
  //iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
  iResult = getaddrinfo(DEFAULT_IP, DEFAULT_PORT, &hints, &result);
  if (iResult != 0) {
      printf("getaddrinfo failed: %d\n", iResult);
      WSACleanup();
      while(1);
      return 1;
  }
  SOCKET listenSocket = INVALID_SOCKET;

  // Attempt to connect to the first address returned by
  // the call to getaddrinfo
  ptr=result;

  // Create a SOCKET for connecting to server
  listenSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
  if (listenSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    while(1);
    return 1;
  }

  // Setup the TCP listening socket
  iResult = bind( listenSocket, result->ai_addr, (int)result->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
    printf("bind failed with error: %d\n", WSAGetLastError());
    freeaddrinfo(result);
    closesocket(listenSocket);
    WSACleanup();
    while(1);
    return 1;
  }

  freeaddrinfo(result);

  // listen for a connection from client
  if ( listen( listenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
    printf( "Listen failed with error: %ld\n", WSAGetLastError() );
    closesocket(listenSocket);
    WSACleanup();
    return 1;
  }

  SOCKET ClientSocket;
  ClientSocket = INVALID_SOCKET;

  // Accept a client socket
  ClientSocket = accept(listenSocket, NULL, NULL);
  if (ClientSocket == INVALID_SOCKET) {
    printf("accept failed: %d\n", WSAGetLastError());
    closesocket(listenSocket);
    WSACleanup();
    return 1;
  }

  char recvbuf[DEFAULT_BUFLEN];
  int iSendResult;
  int recvbuflen = DEFAULT_BUFLEN;

  std::ifstream fd_send;
  std::ifstream::pos_type size;
  char* send_block;

  // Receive until the peer shuts down the connection
  do {
    iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0) {
      printf("Bytes received: %d\n", iResult);
      if(strcmp(recvbuf,"file request") != 0) {
        std::cout << "Warning: received invalid request from client" << std::endl;
        break;
      } else {
        std::cout << "Received file request from client" << std::endl;
      }

      // send file to client
      //fd_send.open("F:\\Users\\KJ\\Documents\\Visual Studio 2010\\Projects\\winsock\\server_send_file.txt",  std::ios::in|std::ios::binary|std::ios::ate);
      fd_send.open("F:\\Users\\KJ\\Documents\\Visual Studio 2010\\Projects\\winsock\\10 Sung Shi Kiung - You're My Spring.mp3",  std::ios::in|std::ios::binary|std::ios::ate);
      
      if(fd_send.is_open()) {
        std::cout << "file open for sending" << std::endl;
      } else {
        std::cout << "file failed to open for sending" << std::endl;
        break;
      }
      size = fd_send.tellg();
      send_block = new char [size];
      fd_send.seekg (0, std::ios::beg);
      fd_send.read (send_block, size);
      fd_send.close();

      std::cout << "the complete file content is in memory" << std::endl;

      iSendResult = send(ClientSocket, send_block, size, 0);
      if (iSendResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
      }
      printf("Bytes sent: %d\n", iSendResult);
      delete[] send_block;

    } else if (iResult == 0)
      printf("Connection closing...\n");
    else {
      printf("recv failed: %d\n", WSAGetLastError());
      closesocket(ClientSocket);
      WSACleanup();
      return 1;
    }

  } while (iResult > 0);

  // shutdown the connection since we're done
  iResult = shutdown(ClientSocket, SD_SEND);
  if (iResult == SOCKET_ERROR) {
    printf("shutdown failed with error: %d\n", WSAGetLastError());
    closesocket(ClientSocket);
    WSACleanup();
    return 1;
  }

  // cleanup
  closesocket(listenSocket);
  WSACleanup();

  while(1);
	return 0;
}

