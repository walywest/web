/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServerIncludes.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkarib <gkarib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 21:42:11 by gkarib            #+#    #+#             */
/*   Updated: 2023/07/08 16:50:29 by gkarib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVERINCLUDES_HPP
#define WEBSERVERINCLUDES_HPP

class   Location;   

class   ServerConfigParser;

class	HTTPserverParsing;

class	ConfigFile;


#include "Libraries.hpp"

#include "ConfigFileParsing/Location.hpp"

#include "ConfigFileParsing/ServerConfig.hpp"

#include "ConfigFileParsing/ConfigFile.hpp"


// #define c_num 1
#define _port 8080
#define localhost "127.0.0.1"
#define backlog 10
#define test_response "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"
#define test_root "../webzeb"
#define M_H 8000
#define M_B 1502091619
#define UPLOADED_FILE "fototita.mp4"
#define	FILE_SIZE 1502091619
#include "post.hpp"


class httpServer
{
	private:
		/*-------------------------------Data  memebers----------------------------*/
		// std::vector<serverSocket>			server_socket; // socket descriptor, an integer (like a file-handle)
		struct sockaddr_in	address;
		socklen_t			addrLength;
		int					serverSocket; // socket descriptor, an integer (like a file-handle)
		int									status;
		std::vector<ServerConfig>			data;
		std::map<int,std::string>			status_phrase; // temporary solution
		/*--------------------------Initiating the connection-----------------------*/
		void	startingConnection(int domain, int type, int protocol, int port);
		/*--------------------------Send and receive messages-----------------------*/
		void						startingServer();
		/*--------------------------Mini parser for the request-----------------------*/
		void	parseRequest(char* buffer, pars &p, ServerConfig data);
		void						is_valid(std::map<std::string,std::string> headers, std::string method, std::string url, std::string httpVersion);
		std::string					getRoot(Location rootLocation, std::string serverRoot,const Location matchedLocation);
		std::string					getPath(std::string url, const Location* matchedLocation, std::string root);
		const Location*				getMatchingLocation(std::string url, std::string method,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        const std::vector<Location>& location);
		std::string					getContent(std::string filename);
		std::vector<std::string>	getDirContent(std::string path);
		std::string					generateDirectoryListing(const std::vector<std::string>& dirContent);
		void						setStatusPhrase(void);
		/*-------------------------------Response generator----------------------------*/
		void						generateResponse(std::string s, std::string type);
		/*----------------------------------HTTP methods-------------------------------*/
		void						GET(std::string& url, std::map<std::string,std::string> headers, const Location* matchedLocation, ServerConfig data); // i will just ignore the host header for the moment
		void						POST(std::string body, pars &p, const Location* matchedLocation, ServerConfig data);
		void						DELETE(std::string& url, std::map<std::string,std::string> headers, const Location* matchedLocation, ServerConfig data);
		/*----------------------------Common Gateway Interface---------------------------*/
		// void		CGI(std::string query, std::string url, char** env);
		// char**		getEnv(std::map<std::string, std::string>& headers);
	public:
		/*--------------------------------Canonical form-----------------------------*/
		httpServer();
		httpServer(std::vector<ServerConfig> _servers);
		httpServer(httpServer const & obj);
		void	startingConnection(const char* interface, int port);
		~httpServer();
		// httpServer&    operator=(httpServer const & obj);
		/*------------------------------------getters---------------------------------*/
};

#endif