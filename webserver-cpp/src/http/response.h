#ifndef __HTTP_RESPONSE_H
#define __HTTP_RESPONSE_H

#include <string>
#include <sstream>
#include <map>
#include <vector>

namespace http
{
using namespace std;

struct ResponseRaw
{
    char *buf = NULL;
    long len = 0;
};

class HttpResponse
{
  private:
    int statusCode = 200;
    string statusText = "OK";
    map<string, string> headers;
    vector<char> body;

    string _getStatus();
    string _getHeader();
    string _getBody();

  public:
    HttpResponse();

    void setStatus(int, string statusText = "");
    void setHeader(const string &, const string &);
    void write(const char *body);

    void sendFile(const char *filename, int start = 0, int end = 0);

    ResponseRaw getResponse();
};
} // namespace http

#endif