#ifndef __HTTP_RESPONSE_H
#define __HTTP_RESPONSE_H

#include <string>
#include <sstream>
#include <map>
#include <vector>

namespace http {
    using namespace std;

    struct ResponseRaw {
        char *buf = nullptr;
        long len = 0;
    };

#define _s static const char *
    namespace HttpHeader {
        _s ContentType = "Content-Type";
        _s AccessControlAllowOrigin = "Access-Control-Allow-Origin";
        _s Range = "Range";
        _s AcceptRange = "Accept-Ranges";
        _s ContentLength = "Content-Length";
        _s ContentRange = "Content-Range";
    };

    enum HttpStatus {
        OK = 200,
        ParticleContent = 206,
        PermissionDenied = 403,
        NotFound = 404,
        ServerFault = 500
    };
#undef _s

    class HttpResponse {
    private:
        int statusCode = 200;
        string statusText = "OK";
        map <string, string> headers;
        vector<char> body;

        string _getStatus();

        string _getHeader();

        string _getBody();

    public:
        HttpResponse();

        int getStatus();
        void setStatus(int, string statusText = "");

        void setHeader(const string &, const string &);

        void write(const char *body);

        void sendFile(const char *filename, long start = 0, long end = 0);

        ResponseRaw getResponse();
    };
} // namespace http

#endif