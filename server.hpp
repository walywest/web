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
#include <string.h>
#include <sstream>
#include <fstream>
#include <cstring>
#include <map>
#include <sys/poll.h>

#define _port 6969
#define localhost "127.0.0.1"
#define backlog 10
#define test_response "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"
#define test_root "../webzeb"
#define c_num 1

class server {
	private:
		/*-------------------------------Data  memebers----------------------------*/
		int									serverSocket; // socket descriptor, an integer (like a file-handle)
		int									clientSocket;
		struct sockaddr_in					address;
		socklen_t							addrLength;
		/*--------------------------Initiating the connection-----------------------*/
		void	startingConnection(int domain, int type, int protocol, const char* interface, int port);
		/*--------------------------Send and receive messages-----------------------*/
		void	startingServer();
		/*--------------------------Mini parser for the request-----------------------*/
		void			parseRequest(char* buffer);
		std::string		getContent(std::string filename);
		// std::string		getPath(std::string ref, std::string url); // fix the radditional requested files
		/*-------------------------------Response generator----------------------------*/
		void	generateResponse(std::string s, std::string type);
		/*----------------------------------HTTP methods-------------------------------*/
		void	GET(std::string& url); // i will just ignore the host header for the moment
		void	POST(std::string url, std::string body, std::map<std::string,std::string> headers);
		void	DELETE();

	public:
		/*--------------------------------Canonical form-----------------------------*/
		server();
		server(int domain, int type, int protocol, const char* interface, int port);
		server(server const & obj);
		~server();
		server&    operator=(server const & obj);
		/*------------------------------------getters---------------------------------*/
		int	get_sockeFd();
		struct sockaddr_in	get_address();
};

int	ft_strlen(char* str);

#endif