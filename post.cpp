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

void    rm_hexa(pars &p, std::string &body)
{
    if (!p.chunk_n)
    {
        if ((p.hex_l = body.find("\r\n")) != std::string::npos)
        {
            p.hex_l += 2;
            p.hexa = body.substr(0, p.hex_l);
            std::cout << "this is the hexa as a string ===>|" << p.hexa << "|" << std::endl;
            std::istringstream s(p.hexa);
            if (!(s >> std::hex >> p.chunk_n))
                throw std::runtime_error(strerror(errno));
            char    c = s.peek();
            std::cout << "this the first char after hexa |" << c << "|" << std::endl;
            // p.chunk_n = p.body_chunk.size();
            body.erase(0,p.hex_l);
            p.last_h = p.valread - p.hex_l;
            std::cout << "this is the hexa ===>|" << p.chunk_n << "|" << std::endl;
            std::cout << "this is the body now ===>|" << body << "|" << std::endl;
        }
        else
            throw std::runtime_error("MALFORMED RESPONSE: HEXA NOT FOLLOWED BY '\n' '\r'");
        throw std::runtime_error("\n\n\n\n**********SALINA*********\n\n\n\n");
    }
    else
    {
    }
}


pars::pars()
{
    type = 0; 
    t_valread = 0;
    valread = 0;
    cont_l = 0;
    max = M_H;
    p_h = 0;
    s = 0;
    chunk_n = 0;
}

void	pars::upd_valread()
{
    t_valread += s;
    std::cout << "witten " << t_valread << std::endl;
    fflush(stdout);
    if (t_valread > M_B)
    {
       perror("max value reached ===>"); 
       std::cout << max << "< " << t_valread << " in p_h = " << p_h << std::endl;
       throw std::runtime_error(strerror(errno));
    }
}

void	server::POST(std::string body, pars &p) {

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
    if (!(p.headers.find("Transfer-Encoding") == p.headers.end()))
    {
        rm_hexa(p, body);
        throw std::runtime_error("WENT TO CHUNKED");
    }
    else
    {
        p.s = body.size();
        if (p.t_valread <= FILE_SIZE)
        {
            p.upload_file.write(body.c_str(), p.s);
            p.upload_file.flush();
            p.upd_valread();
            //send HTTP OK 2000 here;
         }
         if (p.t_valread == FILE_SIZE) 
         {
            std::cout << "\n\n\n****************"  << "sf ra salat" << "*********" << std::endl;
            fflush(stdout);
            p.upload_file.close();
         }
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
	(void)body;
}
// ****** END OF POST METHOD 