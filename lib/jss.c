#include "jss.h"

int JSS_init() {
  WSADATA info;
  return WSAStartup(MAKEWORD(2, 2), &info);
}

int JSS_cleanup() {
  return WSACleanup();
}

int bindsock(Socket* sock, char* ip, u_short port) {
  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr.S_un = {.S_addr = inet_addr(ip)}
  };
  int r = bind(sock->__socket, (const struct sockaddr*)&addr, sizeof(addr));
  if (r) {
    return r;
  }
  return 0;
}

int listensock(Socket* sock, int backlog) {
  return listen(sock->__socket, backlog);
}

Socket acceptsock(Socket* sock) {
  struct sockaddr_in client_addr;
  int client_len = sizeof(client_addr);
  int client_sock = accept(sock->__socket, (struct sockaddr*)&client_addr, &client_len);

  char ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
  int port = ntohs(client_addr.sin_port);

  return (Socket) {
    .__socket = client_sock,
    .ip = ip,
    .vtable = &DEFAULT_VTABLE,
    .port = port,
    .integrity = hash(&client_sock, sizeof(int))
  };
}

int connectsock(Socket* sock, char* ip, u_short port) {
  struct addrinfo hints = {0}, *res;

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  char prt[5];
  sprintf(prt, "%hu", port);

  getaddrinfo(ip, prt, &hints, &res);

  int r = connect(sock->__socket, res->ai_addr, res->ai_addrlen);
  if (r) {
    return r;
  }
  return 0;
}

int sendsock(Socket* sock, bytes buffer, size_t length) {
  int n = send(sock->__socket, (const char*)buffer, length, 0);
  return n == SOCKET_ERROR ? -WSAGetLastError() : n;
}

Packet recvsock(Socket* sock, size_t length) {
  bytes temp = (bytes)malloc(length);
  int r = recv(sock->__socket, (char*)temp, length, 0);
  if (r == 0) {
    free(temp);
    return (Packet) {
      .bytes = NULL,
      .status = 0,
      .length = 0, 
    };
  }

  if (r < 0) {
    free(temp);
    return (Packet) {
      .bytes = NULL,
      .status = WSAGetLastError(),
      .length = 0
    };
  }
  return (Packet) {
    .bytes = temp,
    .status = 0,
    .length = r,
  };
}

void releasePacket(Packet* packet) {
  free(packet->bytes);
}

Socket newSocket() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  return (Socket) {
    .__socket = sock,
    .vtable = &DEFAULT_VTABLE,
    .integrity = hash(&sock, sizeof(int)),
  };
}
