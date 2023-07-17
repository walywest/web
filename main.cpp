#include "server.hpp"

int main() {
    while (1)
    {
        try {
            server  test;
        } catch (...) {
            // std::cerr << e.what() << std::endl;
            std::cout << "\n\nclosing the socket here -----<<<<<\n\n" << std::endl;
            _exit(0);
        }
    }
}