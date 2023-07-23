/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_file.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkarib <gkarib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:54:03 by gkarib            #+#    #+#             */
/*   Updated: 2023/07/16 09:58:23 by gkarib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/WebServerIncludes.hpp"

ConfigFile::ConfigFile()
{
}

ConfigFile::~ConfigFile()
{
}

ConfigFile::ConfigFile (const ConfigFile& copy)
{
	this->_Servers = copy._Servers;
}


ConfigFile& ConfigFile::operator=(const ConfigFile& copy)
{
	this->_Servers = copy._Servers;
	return (*this);
}

// --------------------------------- GETTERS ---------------------------------

std::vector<ServerConfig>   ConfigFile::GetServers( void )  const
{
	return (this->_Servers);
}

// --------------------------------- SETTERS ---------------------------------

void    ConfigFile::SetServers(std::vector<ServerConfig> Server)
{
	this->_Servers = Server;
}

// --------------------------------- OTHER FUNCTIONS ---------------------------------

bool    isServerAdded(const ServerConfig& server, const std::vector<ServerConfig>& servers)
{
	for (size_t i = 0; i < servers.size(); ++i)
	{
		// Compare the relevant fields to determine if the server is already added
		if (server.GetHost() == servers[i].GetHost() && server.GetPort() == servers[i].GetPort() && server.GetServerNames() == servers[i].GetServerNames() )
			return true;
	}
	return false;
}

void    ConfigFile::read_ConfigFile()
{
	std::string line;
	std::stack<char> brace;
	int close_brace = 0;
	bool serverContextFound = false;
	ServerConfig currentServer;

	while (getline(_Config_file, line))
	{
		line = isComment(line);
		line = trim_white_spaces(line);

		if (!line.empty())
		{
			// std::cout << line << std::endl;
			// check_bracket(line, brace, close_brace);
			
			if (DefineContext(line) == SERVER)
			{

				serverContextFound = true;
				continue ;
			}

			if (DefineContext(line) != SERVER && !serverContextFound)
			{
				std::cerr << "Error: Expected 'server' context not found." << std::endl;
				break;
			}
			
			
			// std::cout << "==>>" << line << std::endl;
			currentServer.fill_ServerInfos(line, _Config_file);
			// std::cout << "DEBUGGING" << std::endl;
			if (line == "}")
			{
				if (!isServerAdded(currentServer, _Servers))
				{
					_Servers.push_back(currentServer);
					currentServer.clearServerConfig(); // Clear the currentServer object for each new server definition
				}
			}
		}
		// std::cout << "***************" << std::endl;
	}
	if (!brace.empty())
	{
		std::cout << "Error: expected unqualified-id '{'" << std::endl;
		exit(1);
	}
	if (close_brace != 0)
	{
		std::cout << "Error: extraneous closing brace '}'" << std::endl;
		exit(1);	
	}
	
	// std::cout << "***********	Size of _Servers in read_ConfigFile: " << _Servers.size() << "	***********\n" << std::endl;

// 	// Print the contents of _Servers vector for debugging
	// for(size_t i = 0; i < _Servers.size(); i++)
	// {
	// 	std::cout << "	Server " << (i + 1) << "\n" << _Servers[i] << std::endl;
	// }
	_Config_file.close();
}


void    ConfigFile::Check_Conf_file(const char *file_name)
{
	if (!check_extension(file_name, ".conf"))
		return;
	open_file(file_name);
	read_ConfigFile();

	// // Print the contents of _Servers vector for debugging
	// std::cout << "***********	Size of _Servers in Check_Conf_file: " << _Servers.size() << "	***********\n" << std::endl;
	// for(size_t i = 0; i < _Servers.size(); i++)
	// {
	// 	std::cout << "	Server " << (i + 1) << "\n" << _Servers[i] << std::endl;
	// }
	// _Config_file.close();
}