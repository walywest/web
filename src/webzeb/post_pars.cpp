#include "server.hpp"

// ********************** POST METHOD 
void    string_split(std::string& m, std::string& s1, std::string& s2, std::string lim)
{
    size_t h_l = 0;

    if ((h_l = m.find(lim)) != std::string::npos)
    {
        s1 += m.substr(0, h_l);
        s2 += m.substr(h_l + lim.size(), m.size());
    }
}

void    server::split_head_body(char *buffer, pars *p)
{
    size_t  h_l;
    std::string sbuff(buffer, p->valread);
    if ((h_l = sbuff.find("\r\n\r\n")) != std::string::npos)
    {
        p->max = M_B;
        p->header += sbuff.substr(0, h_l);
        p->body_chunk += sbuff.substr(h_l + 4, p->valread - h_l - 4);
    }
    else
    {
        perror("mal formed POST request");
		throw std::runtime_error(strerror(errno));
    }
}

int r_err(ssize_t d)
{
    if (d < 0)
    {
        perror("error reading from socket");
		throw std::runtime_error(strerror(errno));
    }
    if (!d)
    {
        perror("server hanging while reading form socket");
		throw std::runtime_error(strerror(errno));
    }
    return (1);
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
        p.upd_valread();
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
    max = M_H;
}

void	pars::upd_valread()
{
    t_valread += valread;
    if (t_valread > max)
    {
       perror("max value reached ===>"); 
       std::cout << max << std::endl;
       throw std::runtime_error(strerror(errno));
    }
}
// ****** END OF POST METHOD 