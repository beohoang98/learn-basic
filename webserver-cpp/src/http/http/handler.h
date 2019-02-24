#ifndef __HANDLER_H
#define __HANDLER_H

#include <string>
#include <regex>

#include "request.h"
#include "response.h"

using namespace std;

namespace http
{

struct HttpHandler
{
    string path;
    string method;
    void (*func)(HttpRequest *req, HttpResponse *res);

    bool match(const HttpRequest &req)
    {
        regex pattern(this->path);

        if (req.method.compare(this->method) == 0)
        {
            if (regex_match(req.path, pattern))
            {
                return true;
            }
        }

        return false;
    }
};

} // namespace http

#endif