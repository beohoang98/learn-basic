#include "http/response.h"
#include "http/request.h"
#include <string>
#include <stdlib.h>
#include <sstream>
#include <cstdio>

using namespace http;
using namespace std;

namespace Controller {

    void hello(HttpRequest * req, HttpResponse * res)
    {
        res->sendFile("views/index.html");
    }

    void NotFound(HttpRequest * req, HttpResponse * res)
    {
        res->setStatus(HttpStatus::NotFound, "Not Found");
        res->sendFile("views/404.html");
    }

    void VideoIndex(HttpRequest * req, HttpResponse * res)
    {
        string range = req->get(HttpHeader::Range, "bytes=0-");
        stringstream ssrange(range);

        long start = 0;
        long end = 0;
        string tmp;
        getline(ssrange, tmp, '=');
        getline(ssrange, tmp, '-');
        start = strtol(tmp.c_str(), nullptr, 10);
        tmp = "";

        ssrange.ignore(1); // -
        getline(ssrange, tmp);
        if (tmp.length() > 0)
        {
            end = strtol(tmp.c_str(), nullptr, 10);
        }

        res->setHeader(HttpHeader::ContentType, "video/mp4");
        res->sendFile("static/video.mp4", start, end);
    }
}
