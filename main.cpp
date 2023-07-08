#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>

#define PORT 8080
size_t  ffstrlen(const char *str)
{
    if (str)
    {
        int i = 0;
        while (str[i])
            i++;
        return (i);
    }
    else
    {
        perror("void access");
        exit(-1);
    }
}
const char    *readfile()
{
    std::string l;
    std::string r;
    std::ifstream k("resp");
    while (getline(k, l))
    {
        l.push_back('\n');
        std::cout << l;
        r = r + l;
        l.clear();
    }
    return (r.c_str());
}

int main(int argc, char const *argv[])
{
    std::ofstream fototita("fototita.jpeg",std::ofstream::binary);

    long long w;
    const char *res = readfile();
    int server_fd, new_socket; long valread = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opts= 1;
    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello Worldadjbfnjdnafbkndklafmvlafbjkadnbfjnsldmvv!";
  
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    memset(&address, '\0', sizeof address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof(int));
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    fcntl(new_socket, F_SETFL, O_NONBLOCK);
    while(1)
    {
        std::cout <<  "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
        fflush(stdout);
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        while (valread)
        {
            char buffer[30000] = {0};
            std::cout << "here" << std::endl;
            fflush(stdout);
            valread = read( new_socket , buffer, 30000);
            if (valread == -1)
            {
                perror("read failed");
                exit(0);
            }
            std::string requ = std::string(buffer, valread);
            // fototita.write(buffer, valread);
            fototita << requ;
            // fflush(fototita);
            w += valread;
            std::cout << buffer;
            std::cout << "\n---the number of written bytes is " << w << std::endl;
            fflush(stdout);
        }
        write(new_socket , res , ffstrlen(res));
        std::cout << "\n------------------Hello message sent-------------------"  << std::endl;
        std::cout << "the connecting entity address is --> : "\
        << address.sin_addr.s_addr << std::endl;
        close(new_socket);
    }
    return 0;
}
