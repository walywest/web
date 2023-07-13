#include "server.hpp"

int main() {
    try {
        server  test;
    } catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        std::cout << "\n\nclosing the socket here -----<<<<<\n\n" << std::endl;
    }
    return 0;
}