#include "server.hpp"

int main() {
    try {
        server  test;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}