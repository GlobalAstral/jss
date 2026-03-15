#pragma once

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <hashing.h>

#define long long long

typedef struct Socket Socket;
typedef unsigned char byte;
typedef byte* bytes;

int JSS_init();
int JSS_cleanup();

typedef struct Packet {
  bytes bytes;
  size_t length;
  int status;
} Packet;

typedef struct SocketVTable {
  int (*bind)(Socket* sock, char* ip, u_short port);
  int (*listen)(Socket* sock, int backlog);
  Socket (*accept)(Socket* sock);
  int (*connect)(Socket* sock, char* ip, u_short port);
  int (*send)(Socket* sock, bytes buffer, size_t length);
  Packet (*recv)(Socket* sock, size_t length);
} SocketVTable;

typedef struct Socket {
  int __socket;
  char* ip;
  u_short port;
  SocketVTable* vtable;
  unsigned long integrity;
} Socket;

int bindsock(Socket* sock, char* ip, u_short port);
int listensock(Socket* sock, int backlog);
Socket acceptsock(Socket* sock);
int connectsock(Socket* sock, char* ip, u_short port);
int sendsock(Socket* sock, bytes buffer, size_t length);
Packet recvsock(Socket* sock, size_t length);
void releasePacket(Packet* packet);

static SocketVTable DEFAULT_VTABLE = {
  .bind = bindsock,
  .listen = listensock,
  .accept = acceptsock,
  .connect = connectsock,
  .send = sendsock,
  .recv = recvsock
};

Socket newSocket();
