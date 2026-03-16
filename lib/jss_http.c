#include "jss_http.h"

HTTPRequest newHTTPRequest() {
  return (HTTPRequest) {
    .headers = newDynamicArray(sizeof(Header)),
  };
}

void httpRequestSetMethod(HTTPRequest* req, HTTPMethod method) {
  req->method = method;
}

void httpRequestSetPath(HTTPRequest* req, char* path) {
  req->target_path = path;
}

void httpRequestSetHttpVersion(HTTPRequest* req, int major, int minor) {
  const size_t ver_len = 20;
  char* ver = (char*)malloc(ver_len);
  sprintf(ver, "HTTP/%d.%d", major, minor);
  req->http_version = ver;
}

void httpRequestSetBody(HTTPRequest* req, char* body) {
  req->body = body;
  req->body_length = strlen(body);
}

void httpRequestAddHeader(HTTPRequest* req, char* name, char* value) {
  Header h = {name, value};
  appendDynamicArray(&req->headers, &h);
}

static const char* http_methods[] = {
  "GET",
  "POST",
  "PUT",
  "PATCH",
  "DELETE",
  "HEAD",
  "OPTIONS",
  "TRACE"
};

char* get_http_method(HTTPMethod method) {
  if (method < 0 || method >= sizeof(http_methods)/sizeof(http_methods[0]))
    return "GET";
  return http_methods[method];
}

Packet httpRequestCompose(HTTPRequest* req) {

  DynamicString headers = ds_new();

  ds_append(&headers, get_http_method(req->method));
  ds_append(&headers, " ");
  ds_append(&headers, req->target_path);
  ds_append(&headers, " ");
  ds_append(&headers, req->http_version);
  ds_append(&headers, "\r\n");

  for (size_t i = 0; i < req->headers.length; i++) {
    Header* phd = (Header*)getDynamicArray(&req->headers, i);
    ds_append(&headers, phd->name);
    ds_append(&headers, ": ");
    ds_append(&headers, phd->value);
    ds_append(&headers, "\r\n");
  }

  ds_append(&headers, "\r\n");
  ds_append(&headers, req->body);
  return (Packet) {
    .bytes = str_of_dyn(&headers),
    .length = headers.length,
    .status = 0
  };
}
