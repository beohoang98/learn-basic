#include "http/httpserver.h"
#include "controller.h"
#include <unistd.h>

using namespace http;
using namespace std;

int main()
{
    // in bin
    #ifdef WORKING_DIR
        chdir(WORKING_DIR);
        printf("Working on dir %s\n", WORKING_DIR);
    #endif

    HttpServer server(8000);
    server.get("/", hello);
    server.get("/404", NotFound);
    server.get("/video", VideoIndex);
    server.useStatic("static");

    server.listen();
    return 0;
}
