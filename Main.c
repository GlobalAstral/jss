#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <jss.h>

int main(int argc, char** argv) {  
  int r = JSS_init();
  if (r) {
    printf("Startup failed with %d\n", r);
  }

  if (argc < 4) {
    puts("Too little cli args");
    return 1;
  }

  bool isclient = strcmp(argv[1], "client") == 0;
  char* ip = argv[2];
  unsigned short port = atoi(argv[3]);

  if (isclient) {
    Socket client = newSocket();
    puts("Socket created");
    
    printf("Connecting to %s:%hu...\n", ip, port);
    client.vtable->connect(&client, ip, port);
    printf("Connected to %s:%hu\n", ip, port);

    unsigned char req[] = "Ah ok, no ma ci sta";
    puts("Sending packet...");
    client.vtable->send(&client, req, sizeof(req));

    puts("Trying receiving packet...");
    Packet packet = client.vtable->recv(&client, 4096);
    if (packet.status) {
      puts("Can't get response");
      return 0;
    }
    printf("res: '%s'\n", packet.bytes);

    releasePacket(&packet);

    return 0;
  }

  Socket server = newSocket();
  puts("Socket created");
  printf("Binding to %s:%hu...\n", ip, port);
  server.vtable->bind(&server, ip, port);
  printf("Binded to %s:%hu\n", ip, port);
  server.vtable->listen(&server, 1);
  puts("Listening...");
  Socket client = server.vtable->accept(&server);
  printf("Client %s:%hu connected.\n", client.ip, client.port);
  Packet packet = client.vtable->recv(&client, 4096);

  if (packet.status) {
    printf("Can't get packet. STATUS: %d", packet.status);
    return 0;
  }
  puts("RECV:");
  for (size_t i = 0; i < packet.length; i++) {
    putchar(packet.bytes[i]);
  }

  releasePacket(&packet);

  unsigned char buffer[] = "if (T == 1)\n\tT = 1";

  client.vtable->send(&client, buffer, sizeof(buffer));

  JSS_cleanup();

	return 0;
}
