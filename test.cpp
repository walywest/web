#include "server.hpp"

int main() {
	int	chunk;
                std::istringstream s("1a");
	std::cout << s.rdbuf()->in_avail() << std::endl;
                if ((s >> std::hex >> chunk))
					std::cout << "this is chunk " << chunk << std::endl;
	std::cout << s.rdbuf()->in_avail() << std::endl;
	std::cout << "and this is it"<< s.str() << std::endl;
}