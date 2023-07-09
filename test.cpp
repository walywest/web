#include <iostream>
#include <unistd.h>
#include <ostream>
#include <fstream>
#include <istream>

int main()
{
    std::ifstream input("dongbiao-lu-small.jpeg");
    std::ofstream output("hehe.jpeg");
    if (input && output)
    {
        input.seekg(0, input.end);
        int size = input.tellg();
        input.seekg(0, input.beg);
        char    *buff = new char[size];
        input.read(buff, size);
        output.write(buff, size);
        delete[] buff;
        input.close();
        output.close();
    }
}