#pragma once

#include <stdbool.h>

#include "jss.h"
#include "listing.h"
#include "actualstrings.h"

typedef enum HTTPMethod {
  HTTP_GET,
  HTTP_POST,
  HTTP_PUT,
  HTTP_PATCH,
  HTTP_DELETE,
  HTTP_HEAD,
  HTTP_OPTIONS,
  HTTP_TRACE
} HTTPMethod;

typedef struct Header {
  char* name;
  char* value;
} Header;

typedef struct HTTPRequest {
  HTTPMethod method;
  char* target_path;
  char* http_version;

  char* body;
  size_t body_length;

  DynamicArray headers;
} HTTPRequest;

HTTPRequest newHTTPRequest();

void httpRequestSetMethod(HTTPRequest* req, HTTPMethod method);
void httpRequestSetPath(HTTPRequest* req, char* path);
void httpRequestSetHttpVersion(HTTPRequest* req, int major, int minor);
void httpRequestSetBody(HTTPRequest* req, char* body);
void httpRequestAddHeader(HTTPRequest* req, char* name, char* value);
Packet httpRequestCompose(HTTPRequest* req);
