// winsock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
#define DEFAULT_IP "192.168.1.117"

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

  // client
  struct addrinfo *result = NULL, *ptr = NULL, hints;

  ZeroMemory( &hints, sizeof(hints) );
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  //iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
  iResult = getaddrinfo(DEFAULT_IP, DEFAULT_PORT, &hints, &result);
  if (iResult != 0) {
      printf("getaddrinfo failed: %d\n", iResult);
      WSACleanup();
      while(1);
      return 1;
  }
  SOCKET ConnectSocket = INVALID_SOCKET;

  // Attempt to connect to the first address returned by
  // the call to getaddrinfo
  ptr=result;

  // Create a SOCKET for connecting to server
  ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
  if (ConnectSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    while(1);
    return 1;
  }

  // Connect to server.
  iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
    closesocket(ConnectSocket);
    ConnectSocket = INVALID_SOCKET;
  }

  // Should really try the next address returned by getaddrinfo
  // if the connect call failed
  // But for this simple example we just free the resources
  // returned by getaddrinfo and print an error message

  freeaddrinfo(result);

  if (ConnectSocket == INVALID_SOCKET) {
    printf("Unable to connect to server!\n");
    WSACleanup();
    while(1);
    return 1;
  }

  int recvbuflen = DEFAULT_BUFLEN;

  char *sendbuf = "file request";
  char recvbuf[DEFAULT_BUFLEN];

  // Send an initial buffer to request a file
  iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf) + 1, 0);
  if (iResult == SOCKET_ERROR) {
    printf("send failed: %d\n", WSAGetLastError());
    closesocket(ConnectSocket);
    WSACleanup();
    return 1;
  }

  printf("Bytes Sent: %ld\n", iResult);

  // shutdown the connection for sending since no more data will be sent
  // the client can still use the ConnectSocket for receiving data
  iResult = shutdown(ConnectSocket, SD_SEND);
  if (iResult == SOCKET_ERROR) {
    printf("shutdown failed: %d\n", WSAGetLastError());
    closesocket(ConnectSocket);
    WSACleanup();
    return 1;
  }

  // file to write to when receiving from server
  std::ofstream fd_receive;
  fd_receive.open("F:\\Users\\KJ\\Documents\\Visual Studio 2010\\Projects\\winsock\\client_receive_file",  std::ios::out|std::ios::binary);

  if(fd_receive.is_open()) {
    std::cout << "file open for writing" << std::endl;
  } else {
    std::cout << "file failed to open for writing" << std::endl;
    return 1;
  }

  // Receive data until the server closes the connection
  do {
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0) {
      printf("Bytes received: %d\n", iResult);
      fd_receive.write(recvbuf, iResult);
      
    }
    else if (iResult == 0)
      printf("Connection closed\n");
    else
      printf("recv failed: %d\n", WSAGetLastError());
  } while (iResult > 0);

  fd_receive.close();

  // cleanup
  closesocket(ConnectSocket);
  WSACleanup();

  while(1);
	return 0;
}

