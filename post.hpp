#ifndef POST_HPP
#define POST_HPP

class pars {
	public :
		int	end_flag;
		size_t		written;
		size_t		to_write;
		size_t		hex_l;
		size_t		to_be_skip;
		std::string	hexa; //the hexa value as a string
		size_t		chunk_n;
		int		type;
		ssize_t t_valread;
		ssize_t valread;
		ssize_t cont_l;
		ssize_t max;
		size_t s;
		std::map<std::string, std::string> headers;
		std::ofstream			upload_file;
		std::string	host;
		pars();
		void	upd_valread();
};

int	r_err(ssize_t d, pars &p);
int		rm_hexa(pars &p, std::string &body);
void    write_content(pars &p, size_t len, char *body);
#endif