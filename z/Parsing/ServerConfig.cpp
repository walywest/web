/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkarib <gkarib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 01:09:24 by ghita             #+#    #+#             */
/*   Updated: 2023/07/20 18:56:53 by gkarib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/WebServerIncludes.hpp"


ServerConfig::ServerConfig()
{
}

ServerConfig::~ServerConfig()
{
}

ServerConfig::ServerConfig(const ServerConfig& other)
{
	this->_Host = other._Host;
	this->_Root = other._Root;
	this->_Port = other._Port;
	this->_ServerNames = other._ServerNames;
	this->_DefaultServer = other._DefaultServer;
	this->_ErrorPages = other._ErrorPages;
	this->_LimitClientBodySize = other._LimitClientBodySize;
	this->_Locations = other._Locations;
}

ServerConfig& ServerConfig::operator=(const ServerConfig& other)
{
	if (this != &other)
	{
		// _Locations.clear();
		// for (size_t i = 0; i < other._Locations.size(); ++i)
		// {
		// 	_Locations.push_back(other._Locations[i]);
		// }

		this->_Host = other._Host;
		this->_Root = other._Root;
		this->_Port = other._Port;
		this->_ServerNames = other._ServerNames;
		this->_DefaultServer = other._DefaultServer;
		this->_ErrorPages = other._ErrorPages;
		this->_LimitClientBodySize = other._LimitClientBodySize;
		this->_Locations = other._Locations;
	}
	return (*this);
}

// --------------------------------- GETTERS ---------------------------------

std::string	ServerConfig::GetHost( void ) const
{
	return (this->_Host);
}

std::string	ServerConfig::GetRoot( void ) const
{
	return(this->_Root);
}

int	ServerConfig::GetPort( void ) const
{
	return (this->_Port);
}

std::vector<std::string>	ServerConfig::GetServerNames( void ) const
{
	return (this->_ServerNames);
}

bool	ServerConfig::GetDefaultServer( void ) const
{
	return (this->_DefaultServer);
}

std::map<int, std::string>	ServerConfig::GetErrorPages( void ) const
{
	return (this->_ErrorPages);
}

std::string	ServerConfig::GetLimitClientBodySize( void ) const
{
	return (this->_LimitClientBodySize);
}

std::vector<Location>	ServerConfig::GetLocation( void ) const
{
	return (this->_Locations);
}

// --------------------------------- SETTERS ---------------------------------

void	ServerConfig::SetHost(const std::string& host)
{
	this->_Host = host;
}

void	ServerConfig::SetRoot(const std::string& root)
{
	this->_Root = root;
}

void	ServerConfig::SetPort(int port) {
	this->_Port = port;
}

void	ServerConfig::SetServerNames(const std::vector<std::string>& serverNames)
{
	this->_ServerNames = serverNames;
}

void	ServerConfig::SetDefaultServer(bool defaultServer)
{
	this->_DefaultServer = defaultServer;
}

void	ServerConfig::SetErrorPages(const std::map<int, std::string>& errorPages)
{
	this->_ErrorPages = errorPages;
}

void	ServerConfig::SetLimitClientBodySize(const std::string& limitClientBodySize)
{
	this->_LimitClientBodySize = limitClientBodySize;
}

void	ServerConfig::SetLocation(const std::vector<Location>& location)
{
	this->_Locations = location;
}

// --------------------------------- OTHER FUNCTIONS ---------------------------------

bool	ServerConfig::isServerConfigFilled()
{		
		return (!GetHost().empty() &&
				!GetRoot().empty() &&
				!GetServerNames().empty() &&
				!GetErrorPages().empty() &&
				!GetLimitClientBodySize().empty() &&
				!GetLocation().empty());
}

void	ServerConfig::clearServerConfig()
{
	// Reset the state of the server object
	SetHost("");
	SetRoot("");
	SetPort(0);
	SetServerNames(std::vector<std::string>());
	SetDefaultServer(false);
	SetErrorPages(std::map<int, std::string>());
	SetLimitClientBodySize("");
	SetLocation(std::vector<Location>());
}

bool	isServerConfigFilled(const Location& location)
{
	return (!location.GetMethods().empty() /*&&
			!location.Get_Redirect().empty() &&
			!location.Get_DefaultFile().empty() &&
			!location.GetCGIextensions().empty() &&
			!location.GetFileUpload().empty() &&
			!server.GetLocation().empty()*/);
}

void	clearLocation(Location& location)
{
	// Reset the state of the server object
	location.SetMethods(std::vector<std::string>());
	location.SetDirectoryListing(0);
	location.SetDefaultFile("");
	location.SetCGIextensions(std::vector<std::string>());
	location.SetDirectory("");
	location.SetFileUpload(std::map<std::string, std::string>());
	// Clear other members as needed
}

bool	ServerConfig::ValidKey(std::string line)
{
	std::istringstream iss(line);
	std::string key;

	iss >> key;
	// std::cout << "key -> " << key << std::endl;
	return (key != "host:"
		&& key != "port:"
		&& key != "root:"
		&& key != "server_names:"
		&& key != "default_server:"
		&& key != "error_pages:"
		&& key != "limit_client_body_size:"
		&& key != "limit_client_body_size:"
		&& key != "location"
		&& key != "}");
}

void	ServerConfig::fill_ServerInfos(const std::string& line, std::ifstream& config_file)
{
	std::string	value;
	// size_t	PosLoc = line.find("location");
	
	value = line.substr(line.find(":") + 1);
	value = trim_white_spaces(value);
	// std::cout << "key *" << key << "*"<<std::endl;

	// std::cout << line << std::endl;
	// test_brace(line);
	
	if (ValidKey(line))
	{
		std::cout << line << std::endl;
		throw std::invalid_argument("Invalid Key in Server config.");
	}
	if (line.find("host") != std::string::npos)
	{
		this->_Host = value;
		
		// std::cout << "host->" << this->_Host << std::endl;
	}
	else if (line.find("root") != std::string::npos)
	{
		// // TODO: if ':' is not found
		// if (line.find(":") != std::string::npos)
		// 	this->_Root = trim_white_spaces(line.substr(line.find(":") + 1));
		// this->_Root = value;
		SetRoot(value);
		// std::cout << "root->" << GetRoot() << std::endl;
	}
	else if (line.find("port") != std::string::npos)
	{
		// this->_Port = std::atoi(value.c_str());
		std::istringstream iss(value);
		// int port_value = 0;
		if (!(iss >> this->_Port) || !iss.eof())
			throw std::runtime_error("Invalid integer for port format: " + value);
		// std::cout << "port->" << this->_Port << std::endl;
	}
	else if (line.find("server_names") != std::string::npos)
	{
		// split the string "value" by white spaces and push them to the stack
		// TODO: understand 
		std::istringstream iss(value);
		std::string s;
		while (iss >> s)
			this->_ServerNames.push_back(s);
		

		// // print
		// std::cout << "server_names->";
		// for (size_t i = 0; i < this->_ServerNames.size(); i++)
		// {
		//     std::cout << this->_ServerNames[i] << " - ";
		// }
		// std::cout << std::endl;
	}
	else if (line.find("default_server") != std::string::npos)
	{
		if (value == "false")
			this->_DefaultServer = false;
		else if (value == "true")
			this->_DefaultServer = true;
		// std::cout << "default server->" << _DefaultServer <<std::endl;
	}

	else if (line.find("error_pages") != std::string::npos)
	{
		std::string error_line;

		while (getline(config_file, error_line))
		{
			error_line = isComment(error_line);
			error_line = trim_white_spaces(error_line);
			
			if (error_line == "}")
				break;

			// size_t found = error_line.find(":");
			if (error_line.find(":") != std::string::npos)
			{
				std::string error_code = error_line.substr(0, error_line.find(":"));
				std::string error_path = error_line.substr(error_line.find(":") + 1);
				
				// int code = std::atoi(trim_white_spaces(error_code).c_str());
				std::istringstream iss(error_code);
				int code = 0;
				if (!(iss >> code) || !iss.eof())
					throw std::runtime_error("Invalid integer for Error Pages format: " + error_code);
				this->_ErrorPages[code] = trim_white_spaces(error_path);
			}
		}
		// // Print the error_pages
		// std::cout << "error_pages=>";
		// std::map<int, std::string>::const_iterator iter;
		// for (iter = _ErrorPages.begin(); iter != _ErrorPages.end(); ++iter)
		// 	std::cout << "code->" << iter->first << ", error_path->" << iter->second << std::endl;
		// std::cout << std::endl;
	}
	else if (line.find("limit_client_body_size") != std::string::npos)
	{
		this->_LimitClientBodySize = value;
		// std::cout << "limit_client_body_size->" << this->_LimitClientBodySize << std::endl;
	}
	else if (line.find("location") != std::string::npos)
	{
		Location location;
		// std::cout << "********************" << PosLoc << std::endl;
		location.fill_locationInfos(config_file, line);

		this->_Locations.push_back(location);

		// size_t size = GetLocation().size();
		// std::cout << "size = " <<size << std::endl;
		// for (size_t i = 0; i < size; i++)
		// 	std::cout << "location " << i + 1 << _Locations[i] << std::endl;
		
		// std::cout << _Locations[1] << std::endl;
	}
	else if (line != "}")
	{
		std::cout << line << std::endl;
		throw std::invalid_argument("Invalid line in Server config.");
	}

	// return ;
}