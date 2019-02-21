#include "http/response.h"
#include "http/request.h"
#include <string>
#include <sstream>
#include <cstdio>

using namespace http;
using namespace std;

HttpResponse hello(HttpRequest *req)
{
    HttpResponse res = HttpResponse();
    res.sendFile("views/index.html");
    return res;
}
HttpResponse NotFound(HttpRequest *req)
{
    HttpResponse res = HttpResponse();
    res.setStatus(404);
    res.sendFile("views/404.html");
    return res;
}

HttpResponse VideoIndex(HttpRequest *req)
{
    string range = req->headers["Range"];
    stringstream ssrange(range);

    int start = 0;
    int end = 0;
    string tmp;
    getline(ssrange, tmp, '=');
    getline(ssrange, tmp, '-');
    start = atoi(tmp.c_str());
    tmp = "";

    ssrange.ignore(1); // -
    getline(ssrange, tmp);
    if (tmp.length() > 0)
    {
        end = atoi(tmp.c_str());
    }

    HttpResponse res;
    res.setHeader("Content-Type", "video/mp4");
    res.sendFile("static/video.mp4", start, end);
    return res;
}