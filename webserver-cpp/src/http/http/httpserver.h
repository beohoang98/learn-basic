#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <exception>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <regex>

#include "response.h"
#include "request.h"
#include "handler.h"

#define BUFFER_SIZE 2048

using namespace std;

namespace http
{

class ServerException : public exception
{
  public:
    string msg;
    ServerException(string msg)
    {
        this->msg = msg;
    }

    const char *what()
    {
        return this->msg.c_str();
    }
};

class HttpServer
{
  private:
    int port;
    int sockfd;
    struct sockaddr_in se_addr;

    vector<HttpHandler> listHandler;

  public:
    HttpServer(int port);
    void listen(void (*callback)(int port) = nullptr);

    void handle();

    void get(string path, void (*callback)(HttpRequest *, HttpResponse *));
    void useStatic(string path);
};

} // namespace httpserver

#endif