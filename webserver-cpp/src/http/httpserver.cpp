#include <malloc.h>
#include <cstdio>
#include <memory.h>

#include "http/httpserver.h"
#include "http/handler.h"

using namespace http;
using namespace std;

HttpServer::HttpServer(int port)
{
    this->port = port;
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        throw new ServerException("Cannot create socket");
    }
    ((char *)&se_addr, sizeof(se_addr));

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&se_addr, 0, sizeof(se_addr));
    se_addr.sin_family = AF_INET;
    se_addr.sin_addr.s_addr = INADDR_ANY;
    se_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&se_addr, sizeof(se_addr)) < 0)
    {
        printf("Error when bind socket");
        exit(1);
    }
}

void HttpServer::listen(void (*callback)(int port))
{
    if (::listen(sockfd, 100) < 0)
    {
        exit(1);
    }
    if (callback != nullptr) callback(port);

    while (true)
    {
        struct sockaddr_storage serverStorage;
        socklen_t addr_size = sizeof serverStorage;
        int newSocket = accept(sockfd, (struct sockaddr *)&serverStorage, &addr_size);

        char buffer[BUFFER_SIZE] = {0};
        string content;
        int get = 1;
        while ((get = read(newSocket, buffer, BUFFER_SIZE - 1) > 0) && get > 0)
        {
            content += buffer;
            memset(&buffer[0], 0, BUFFER_SIZE);
            if (get < BUFFER_SIZE)
                break;
        }

        HttpRequest request = HttpRequest();
        request.parse(content);
        bool isMatch = false;

        for (HttpHandler handler : listHandler)
        {
            if (handler.match(request))
            {
                isMatch = true;
                HttpResponse res;
                handler.func(&request, &res);

                ResponseRaw raw = res.getResponse();
                write(newSocket, raw.buf, raw.len);
                free(raw.buf);

                printf("%s (%d): %s\n", request.method.c_str(), res.getStatus(), request.path.c_str());
            }
        }

        if (!isMatch)
        {
            HttpResponse res = HttpResponse();
            res.setStatus(404, "Not Found");
            res.write("Not Found");

            ResponseRaw raw = res.getResponse();
            write(newSocket, raw.buf, raw.len);
            free(raw.buf);

            printf("%s (%d): %s\n", request.method.c_str(), 404, request.path.c_str());
        }
        close(newSocket);
    }
}

void HttpServer::get(string path, void (*callback)(HttpRequest *, HttpResponse *))
{
    HttpHandler handler;
    handler.path = path;
    handler.method = "GET";
    handler.func = callback;

    this->listHandler.push_back(handler);
}

void HttpServer::useStatic(string path)
{
    HttpHandler handler;
    handler.path = "\\/" + path + "\\/.*";
    handler.method = "GET";
    handler.func = [] (HttpRequest * req, HttpResponse * res) {
        string filePath = req->path;
        filePath = filePath.erase(0, 1);
        res->sendFile(filePath.c_str());
        res->setHeader("Content-Type", "text/plain");
    };

    this->listHandler.push_back(handler);
}