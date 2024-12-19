#include <string>

#include "test_http_cnc_server.hpp"

using namespace cnc_server;

int main(void) {
    const std::string id = "cnc_server";
    const std::string uri = "http://127.0.0.1:5000";
    const unsigned int timeout_s = 20;
    cnc_server::TestHttpCncServer<std::string> server(id, uri, timeout_s);
    server.run();
    while (server.is_running()) {
        sleep(10);
    }
    return 0;
}