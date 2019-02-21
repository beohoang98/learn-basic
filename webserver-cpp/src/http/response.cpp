#include "response.h"
#include <fstream>
#include <ctime>
#include <cstring>

using namespace http;
using namespace std;


// 
HttpResponse::HttpResponse()
{
    this->setHeader("Status", "OK");
    this->setHeader("Cache-Control", "max-age=3600");
    this->setHeader("Content-Type", "text/html");
    this->setHeader("X-Powered-By", "C/C++");
    this->setHeader("Encoding", "none");
    this->setHeader("Time", to_string(time(NULL)).c_str());
    this->body.clear();
}


// SETTER

void HttpResponse::setStatus(int status, string statusText)
{
    this->statusCode = status;
    this->statusText = statusText;
}

void HttpResponse::setHeader(const string& key, const string& value)
{
    this->headers[key] = value;
}

void HttpResponse::write(const char * body)
{
    string bodyStr = body;
    this->body.insert(this->body.end(), bodyStr.begin(), bodyStr.end());
}

void HttpResponse::sendFile(const char * filename, int start, int end)
{
    ifstream fi (filename, ios::binary);
    if (!fi.is_open()) {
        string msgStr = string(filename) + " is not exists";
        this->body.clear();
        this->body.insert(this->body.end(), msgStr.begin(), msgStr.end());
        return;
    }

    long filesize = fi.tellg();
    fi.seekg(0, ios::end);
    filesize = fi.tellg() - filesize;
    fi.seekg(0, ios::beg);

    long contentLength = filesize;
    if (end <= start) {
        end = filesize - 1;
    }
    contentLength = end - start + 1;

    char * data = new char[contentLength];
    fi.seekg(start);
    fi.read(data, contentLength);
    fi.close();

    this->body.clear();
    for (int i = 0; i < contentLength; ++i) this->body.push_back(data[i]);

    if (contentLength < filesize) {
        this->setStatus(206, "Particle Content");
    }
    this->setHeader("Accept-Ranges", "bytes");
    this->setHeader("Content-Length", to_string(contentLength));
    this->setHeader("Content-Range", (string)"bytes " + to_string(start) + "-" + to_string(end) + "/" + to_string(filesize));
    free(data);
}

// PRIVATE

string HttpResponse::_getStatus()
{
    return (string)"HTTP/1.1 " + to_string(this->statusCode) + " " + this->statusText + "\r\n";
}

string HttpResponse::_getHeader()
{
    string result;
    for (auto keyval : headers)
    {
        result += keyval.first + ": " + keyval.second + "\r\n";
    }
    return result + "\r\n";
}

ResponseRaw HttpResponse::getResponse()
{
    string head = (string) this->_getStatus() + this->_getHeader();
    long lenHead = head.size();
    long lenBody = this->body.size();

    char * buf = new char[lenHead + lenBody];
    strcpy(buf, head.c_str());
    for (long i = 0; i < lenBody; ++i) {
        buf[i + lenHead] = this->body[i];
    }

    ResponseRaw raw;
    raw.buf = buf;
    raw.len = lenHead + lenBody;

    return raw;
}