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
		throw std::runtime_error("tooooz");
    }
    if (!d)
    {
        perror("server hanging while reading form socket");
		throw std::runtime_error("tooooz");
    }
    (void)p;
    return (1);
}

void    write_content(pars &p, size_t len, char *body)
{

        p.s = len;
        if (p.t_valread <= FILE_SIZE)
        {
            p.upload_file.write(body, p.s);
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
void    str_to_hexa(pars& p)
{
    std::istringstream s(p.hexa);
    if (!(s >> std::hex >> p.chunk_n))
        throw std::runtime_error("MALFORMED RESPONSE"); 
}

int check_hexa (pars &p, std::string &body)
{
    size_t rn;
        if ((rn = body.find("\r\n")) != std::string::npos && body.size() >= 2)
        {
            if (rn != 0) //in this case append and apply;
            {
                if (!p.to_be_skip)
                    throw std::runtime_error("MALFORMED RESPONSE");
                std::string tmp_hex = body.substr(0, rn + 2);
                p.hexa += tmp_hex;
                p.hex_l = rn + 2;
                str_to_hexa(p);
                if (!p.chunk_n)
                {
                    p.end_flag = 1;
                    return (0);
                }
                return (1);
            }
            else
            {
                body.erase(0,2);
                if ((rn = body.find("\r\n")) == std::string::npos)
                {
                    p.to_be_skip = 1;
                    p.hexa = body.substr(0, std::string::npos);
                    p.hexa = "\r\n" + p.hexa;
                    p.hex_l = rn + 2;
                    str_to_hexa(p);
                    if (!p.chunk_n)
                    {
                        p.end_flag = 1;
                        return (0);
                    }
                    return (1);
                }
                else
                {
                    p.hexa = body.substr(0,rn + 2);
                    p.hex_l = rn + 4;
                    str_to_hexa(p);
                    if (!p.chunk_n)
                    {
                        p.end_flag = 1;
                        return (0);
                    }
                    return (1);
                }
            }
        }
        else
        {
            std::cout << "BAD REQUEST !!" << std::endl;
            throw std::runtime_error("MALFORMED RESPONSE: HEXA NOT FOLLOWED BY '\n' '\r'");
        }

}

int rm_hexa(pars &p, std::string &body)
{
    char    *b = const_cast<char *> (body.c_str());
    size_t  stream_size = body.size();

    p.written = 0;
    if (p.chunk_n != -1)
    {
        std::string tmp(b, stream_size);
        if (!check_hexa(p, tmp))
            return (0);
        stream_size -= p.hex_l;
        b += p.hex_l;
    }
            //********AFTER REMOVIG THE FIRST HEXA


            while (stream_size > 0)
            {
                p.to_be_skip = 0;
                p.to_write = std::min(p.chunk_n - p.written, stream_size);
                std::cout << "this is to_write " << p.to_write << std::endl;
                write_content(p, p.to_write, b);

                stream_size -= p.to_write;
                    if (stream_size == std::string::npos)
                        throw std::runtime_error("tooooz");
                b += p.to_write;

                if (p.to_write + p.written == p.chunk_n)
                {
                    std::string tmp(b, stream_size);
                    if (!check_hexa(p, tmp))
                        break;
                    p.written = 0;
                    stream_size -= p.hex_l;
                    if (stream_size == std::string::npos)
                        throw std::runtime_error("tooooz");
                    b += p.hex_l; //check later;
                }
                else
                {
                    p.to_be_skip = 0;
                    p.written += p.to_write;
                }
            }
}


pars::pars()
{
    end_flag = -1;
    type = 0; 
    t_valread = 0;
    valread = 0;
    cont_l = 0;
    max = M_H;
    s = 0;
    chunk_n = -1;
    to_be_skip = 0;
}

void	pars::upd_valread()
{
    t_valread += s;
    std::cout << "witten " << t_valread << std::endl;
    fflush(stdout);
    if (t_valread > M_B)
    {
       perror("max value reached ===>"); 
       std::cout << max << std::endl;
       throw std::runtime_error("tooooz");
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
            throw std::runtime_error("tooooz");
        }
    }
    if (!(p.headers.find("Transfer-Encoding") == p.headers.end()) && p.headers["Transfer-Encoding"] == " chunked")
    {
        // std::cout << "parsing the buffer number "
        std::cout << "WENT TO CHUNKED" << std::endl;
        rm_hexa(p, body);
        // throw std::runtime_error("WENT TO CHUNKED");
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