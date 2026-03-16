from http.server import BaseHTTPRequestHandler, HTTPServer

class EchoHandler(BaseHTTPRequestHandler):
  def do_POST(self):
    content_length = int(self.headers.get('Content-Length', 0))
    body = self.rfile.read(content_length)
    self.send_response(200)
    self.send_header('Content-Type', 'application/json')
    self.end_headers()
    self.wfile.write(body)

HTTPServer(('localhost', 8080), EchoHandler).serve_forever()
