#include <malloc.h>
#include <cstdio>
#include <memory.h>

#include "httpserver.h"
#include "handler.h"

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

void HttpServer::listen()
{
    if (::listen(sockfd, 5) < 0)
    {
        exit(1);
    }
    printf("\x1b[32m Server listen at http://0.0.0.0:%d \x1b[0m\n", port);

    while (true)
    {
        struct sockaddr_storage serverStorage;
        socklen_t addr_size = sizeof serverStorage;
        int newSocket = accept(sockfd, (struct sockaddr *)&serverStorage, &addr_size);

        char buffer[BUFFER_SIZE] = {0};
        string content;
        int get = 1;
        while (get = read(newSocket, buffer, BUFFER_SIZE - 1) && get > 0)
        {
            // printf("%s", buffer);
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
                HttpResponse res = handler.func(&request);
                ResponseRaw raw = res.getResponse();
                write(newSocket, raw.buf, raw.len);
                free(raw.buf);
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
        }
        close(newSocket);
    }
}

void HttpServer::get(string path, HttpResponse (*callback)(HttpRequest *))
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
    handler.func = [] (HttpRequest * req) -> HttpResponse {
        HttpResponse res;
        string filePath = req->path;
        filePath = filePath.erase(0, 1);
        res.sendFile(filePath.c_str());
        res.setHeader("Content-Type", "text/plain");

        return res;
    };

    this->listHandler.push_back(handler);
}