#include "http/httpserver.h"
#include "controller.h"
#include <unistd.h>

using namespace http;
using namespace std;

int main()
{
    printf("Working on dir %s\n", get_current_dir_name());

    HttpServer server(8000);
    server.get("/", Controller::hello);
    server.get("/404", Controller::NotFound);
    server.get("/video", Controller::VideoIndex);
    server.useStatic("static");

    server.listen([](int port) {
        printf("\x1b[32mServer listen at http://0.0.0.0:%d \x1b[0m\n", port);
    });
    return 0;
}
