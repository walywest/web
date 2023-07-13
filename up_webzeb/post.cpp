
#include "server.hpp"

// ********************** POST METHOD 
int string_split(std::string& m, std::string& s1, std::string& s2, std::string& lim)
{
    size_t h_l = 0;

    if ((h_l = m.find(lim)) != std::string::npos)
    {
        s1 += m.substr(0, h_l);
        s2 += m.substr(h_l + lim.size());
        return (1);
    }
    return (0);
}

int r_err(ssize_t d, pars &p)
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
    (void)p;
    return (1);
}

// void    rm_hexa(pars &p)
// {
//     std::istringstream body(p.body_chunk);
//     if (p.body_chunk.find("\r\n") != 0)
//     {

//     }
//     else
//     {
//         perror("Bad Request");
// 		throw std::runtime_error(strerror(errno));
//     }
// }

// void	server::post_parse(pars& p)
// {
// }

pars::pars()
{
    type = 0; 
    t_valread = 0;
    valread = 0;
    cont_l = 0;
    max = M_H;
    p_h = 0;
}

void	pars::upd_valread()
{
    t_valread += s;
    if (t_valread > M_B)
    {
       perror("max value reached ===>"); 
       std::cout << max << "< " << t_valread << " in p_h = " << p_h << std::endl;
       throw std::runtime_error(strerror(errno));
    }
}

void	server::POST(std::string url, std::string body, pars &p) {

    //add the checks here
    /********/ //check if the body doesn't exist
    //later check if the length of the body that was written is equal to the Content-Length

    /********/
    /********/
    if (!p.upload_file.is_open())
    {
        p.upload_file.open(UPLOADED_FILE); //RWV
        if (!p.upload_file)
        {
            perror("failed creating file");
            throw std::runtime_error(strerror(errno));
        }
    }
    if (p.headers.find("Transfer-Encoding") == p.headers.end())
    {
        p.s = body.size();
        if (p.s <= FILE_SIZE)
        {
            p.upd_valread();
            p.upload_file.write(body.c_str(), p.s);
            p.upload_file.flush();
            //send HTTP OK 2000 here;
         }
         if (p.t_valread == FILE_SIZE) 
            p.upload_file.close();
    }
    // else
    // {
    //     if () //check for the chunked header
    //     {
    //         // rm_hexa(p);
    //         std::cout << "right" << std::endl;
    //     }
    //     p.upload_file.write(p.body_chunk.c_str(), p.body_chunk.size());
    //     p.upload_file.flush();
    //     p.body_chunk.clear();
    // }
    (void)p;
	(void)url;
	(void)body;
}
// ****** END OF POST METHOD 