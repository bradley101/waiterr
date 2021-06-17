#include "../src/waiterr.h"

int main() {
    int port = 5000;

    waiterr::Waiter waiter(port);

    waiter.get("/", [](waiterr::Request& req, waiterr::Response& res) {
        res.set_content_type("application/json").set_body("{\"msg\": \"Okay first time, huh!\"}").send();
    });

    waiter.get("/some/shit", [](waiterr::Request& req, waiterr::Response& res) {
        res.set_content_type("application/json").set_body("{\"msg\": \"Okay second time, huh!\"}").send();
    });

    waiter.start_listen([]() {
        std::cout << "In listening mode\n";
    });
    return 0;
}