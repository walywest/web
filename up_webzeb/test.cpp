#include "server.hpp"

int main() {
    DIR*  dir = opendir("../webzeb/src/");
	std::cout <<"hh\n"; 
	if (dir != NULL) {
		struct dirent* entry;
		while ((entry = readdir(dir)) != NULL)  {
			if (entry->d_type == DT_REG) {
				std::cout << entry->d_name << std::endl;
			}
			std::cout <<"here\n";
		}
		if (entry == 0)
			
		closedir(dir);
	}
	else
		std::cout << "l7wa2\n";
    return 0;
}