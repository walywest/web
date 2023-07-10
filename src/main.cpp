#include "rheaders.hpp"

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
    l.push_back('\0');
    k.close();
    return (strdup(l.c_str()));
}

int main()
{

    long long w = 0;
    const char *res = readfile();
    int server_fd, new_socket; long valread = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opts= 1;
  
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
    std::string empt_line = "\r\n\r\n"; 
 
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
// to be copied to POST
//*****************
    std::ofstream fototita("fototita.mp4");
//*****************
    int start;
    int found = 0;
    int flag = 0;
    while(1)
    {
        std::cout <<  "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
// to be copied to POST
//*****************
        while (w < 78594806) //RWV //while should become an if
        {
            flag = 2;
            char *buffer = new char[1024];
            fflush(stdout);
            valread = read( new_socket , buffer, 1024);
            if (valread == -1)
            {
                perror("read failed");
                exit(0);
            }
            if (!valread)
                continue;
            std::string sbuff(buffer, valread);
            if (!found && (start = sbuff.find(empt_line)) != std::string::npos)
            {
                flag = 0;
                found = 1;
                delete[] buffer;
                buffer = const_cast <char *> (sbuff.c_str());
                write(1, buffer, start);
                buffer += (start + 4);
                valread -= (start + 4);
                std::cout << "HEADER=" << start << " BYTES" << std::endl;
            }
            if (found) // check for transer_type
            {
                // std::string requ = std::string(buffer, valread);
                _post(buffer, valread, "likan.mp4"); // RWV
                w += valread;
                // std::cout << requ;
                // std::cout << "\n---the number of written bytes is " << valread << std::endl;
                // std::cout << "\n---the total is " << w << std::endl;
                // fflush(stdout);
            }
            if (flag)
                delete[] buffer;
        }
        fototita.close();
        write(new_socket , res , ffstrlen(res));
        std::cout << "\n------------------Hello message sent-------------------"  << std::endl;
        std::cout << "the connecting entity address is --> : "\
        << address.sin_addr.s_addr << std::endl;
        close(new_socket);
    }
    return 0;
}
