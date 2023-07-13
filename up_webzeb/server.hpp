#ifndef server_HPP
#define server_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <map>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <string>
#include <sys/poll.h>

#define c_num 1
#define _port 8080
#define localhost "127.0.0.1"
#define backlog 10
#define test_response "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"
#define test_root "../webzeb"
#define M_H 8000
#define M_B 386108869
#define UPLOADED_FILE "fototita.mp4"
#define	FILE_SIZE	38610886


class pars {
	public :
		int		p_h;
		int		type;
		size_t	chunk_l;
		std::string	hexa;
		ssize_t t_valread;
		ssize_t valread;
		ssize_t cont_l;
		ssize_t max;
		size_t s;
		std::string	body_chunk;
		std::map<std::string, std::string> headers;
		std::pair<std::string, std::string> key_val;
		std::ofstream			upload_file;
		std::string	host;
		pars();
		void	upd_valread();
};

int	r_err(ssize_t d, pars &p);


void    rm_hexa(pars &p, std::string &sbuf);
class server {
	private:
		/*-------------------------------Data  memebers----------------------------*/
		int					serverSocket; // socket descriptor, an integer (like a file-handle)
		int					clientSocket;
		struct sockaddr_in	address;
		socklen_t			addrLength;
		/*--------------------------Initiating the connection-----------------------*/
		void	startingConnection(int domain, int type, int protocol, int port);
		/*--------------------------Send and receive messages-----------------------*/
		void	startingServer();
		/*--------------------------Mini parser for the request-----------------------*/

		// POST METHOD
		/******/
		// void    string_split(std::string& m, std::string& s1, std::string& s2, std::string& lim);
		int		pre_parse(pars& p);
		void	post_parse(pars& p);
		void    split_head_body(char *buffer, pars &p);
		/******/

		void			parseRequest(char* buffer, pars& p);
		std::string		getContent(std::string filename);
		/*-------------------------------Response generator----------------------------*/
		void	generateResponse(std::string s, std::string type);
		/*----------------------------------HTTP methods-------------------------------*/
		void	GET(std::string& url, std::map<std::string,std::string> headers); // i will just ignore the host header for the moment
		void	POST(std::string url, std::string body, pars& p);
		void	DELETE();

	public:
		/*--------------------------------Canonical form-----------------------------*/
		server();
		server(int domain, int type, int protocol, int port);
		server(server const & obj);
		~server();
		server&    operator=(server const & obj);
		/*------------------------------------getters---------------------------------*/
		int	get_sockeFd();
		struct sockaddr_in	get_address();
};

int		ft_strlen(char* str);

#endif