/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkarib <gkarib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 21:27:09 by gkarib            #+#    #+#             */
/*   Updated: 2023/07/20 18:35:09 by gkarib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERPARSER_HPP
#define SERVERPARSER_HPP

#include "../WebServerIncludes.hpp"

class  ServerConfig //* the informations within the server 
{
	private:
		std::string					_Host;
		std::string					_Root;
		int							_Port;
		std::vector<std::string>	_ServerNames;
		bool						_DefaultServer;
		std::map<int, std::string>	_ErrorPages;
		std::string					_LimitClientBodySize;
		std::vector<Location>		_Locations;
	
	public: 
		ServerConfig();
		~ServerConfig();

		ServerConfig(const ServerConfig& other);
		ServerConfig& operator=(const ServerConfig& other);
		
		// --------------------------------- GETTERS ---------------------------------

		std::string						GetHost( void ) const;
		std::string				    	GetRoot( void ) const;
		int								GetPort( void ) const;
		std::vector<std::string>		GetServerNames( void ) const;
		bool							GetDefaultServer( void ) const;
		std::map<int, std::string>		GetErrorPages( void ) const;
		std::string						GetLimitClientBodySize( void )  const;
		std::vector<Location>    		GetLocation( void ) const;
		
		// --------------------------------- SETTERS ---------------------------------

		void    SetHost(const std::string& host);
		void    SetRoot(const std::string& root);
		void    SetPort(int port);
		void    SetServerNames(const std::vector<std::string>& serverNames);
		void    SetDefaultServer(bool defaultServer);
		void    SetErrorPages(const std::map<int, std::string>& errorPages);
		void    SetLimitClientBodySize(const std::string& limitClientBodySize);
		void    SetLocation(const std::vector<Location>& location);
		
		// --------------------------------- OTHER FUNCTIONS ---------------------------------
		
		void    fill_ServerInfos(const std::string& line, std::ifstream& config_file);
		// void    fill_ServerInfos(const std::string& key, const std::string& line, std::ifstream& config_file);
		bool    isServerConfigFilled();
		void    clearServerConfig();
		bool	ValidKey(std::string line);

};

std::ostream& operator<<(std::ostream& os, const ServerConfig& server);

#endif
