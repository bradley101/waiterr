#include "waiterr.h"

int main() {
    int port = 5000;

    waiterr::Waiter waiter(port);

    waiter.get("/", [](waiterr::Request& req, waiterr::Response& res) {
        std::cout << "In root url\n";

        res.set_content_type("application/json").set_body("{\"msg\": \"Okay first time, huh!\"}").send();
    });

    waiter.start_listen();
    return 0;
}