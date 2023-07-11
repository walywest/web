#include "server.hpp"

// ********************** POST METHOD 

void    server::split_head_body(char *buffer, pars *p)
{
    std::string sbuff(buffer, p->valread);
    if (sbuff.find("\r\n\r\n") != std::string::npos)
    {

    }
    else
    {
        perror("mal formed POST request");
		throw std::runtime_error(strerror(errno));
    }
}

void	server::post_parse(pars& p)
{
    char    buffer[1024] = {0};
    p.valread = read(clientSocket, buffer, 1024);
    if (p.valread < 0)
    {
        perror("error reading from socket");
		throw std::runtime_error(strerror(errno));
    }
    if (p.valread)
    {
        p.t_valread += p.valread;
        std::string sbuff = (buffer, p.valread);
        std::istringstream inp(sbuff);
    }
    else
    {
        perror("server hanging while reading form socket");
		throw std::runtime_error(strerror(errno));
    }
}

pars::pars()
{
    type = 0; 
    t_valread = 0;
    valread = 0;
    cont_l = 0;
}

// ****** END OF POST METHOD 