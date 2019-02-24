#ifndef __HTTP_REQUEST_H
#define __HTTP_REQUEST_H

#include <map>
#include <string>
#include <cstdio>
#include <sstream>


#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 2048
#endif

using namespace std;

namespace http
{
class HttpRequest
{
  public:
    string path;
    map<string, string> headers;
    string method = "GET";

    HttpRequest() {}

    void parse(const string & content)
    {
        stringstream ss;
        ss << content;

        char key[50];
        char value[BUFFER_SIZE];
        string line;

        getline(ss, line);
        stringstream ssline (line);
        ssline >> this->method;
        ssline >> this->path;
        ssline.clear();

        while (!ss.eof())
        {
            getline(ss, line);
            if (line.length() <= 1)
                break; // \r
            sscanf(line.c_str(), "%[^:]: %s", key, value);
            headers[key] = value;
        }

        ss.read(value, ss.gcount());
    }

    string get(const string key, const string val_default = "") {
        auto it = headers.find(key);
        if (it != headers.end()) {
            return it->second;
        } else {
            return val_default;
        }
    }
};
} // namespace http

#endif
