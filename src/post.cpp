#include "rheaders.hpp"

void    _post(char *buffer, size_t len, std::string name)
{
    std::ofstream fototita(name.c_str());
    fototita.write(buffer, len);
}