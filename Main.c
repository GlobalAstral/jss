#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <jss_extended.h>

int main(int argc, char** argv) {  
  int r = JSS_init();
  if (r) {
    printf("Startup failed with %d\n", r);
  }

  HTTPRequest req = newHTTPRequest();

  httpRequestSetMethod(&req, HTTP_GET);
  httpRequestSetPath(&req, "/");
  httpRequestSetHttpVersion(&req, 1, 1);
  httpRequestAddHeader(&req, "Host", "localhost");
  httpRequestSetBody(&req, "{\"name\":\"Alice\"}");

  Packet request = httpRequestCompose(&req);
  printf("REQUEST:\n%s\n", request.bytes);

  Socket sock = newSocket();
  if (sock.vtable->connect(&sock, "127.0.0.1", 8080)) {
    puts("Cannot connect to server");
    return 1;
  }

  sock.vtable->send(&sock, request.bytes, request.length);

  Packet res = sock.vtable->recv(&sock, 4096);
  if (res.status) {
    puts("Cannot get reply");
    return 1;
  }

  printf("RESPONSE:\n%s\n", res.bytes);

  JSS_cleanup();

	return 0;
}
