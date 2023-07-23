/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   overload_operator.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkarib <gkarib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 16:39:15 by gkarib            #+#    #+#             */
/*   Updated: 2023/07/20 18:57:08 by gkarib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/WebServerIncludes.hpp"

std::ostream& operator<<(std::ostream& os, const Location& location)
{
	os << "Path: " << location.GetPath() << std::endl;
	os << "root: " << location.GetRoot() << std::endl;
	os << "index: " << location.GetIndex() << std::endl;
	os << "Methods: ";
	const std::vector<std::string>& _Methods = location.GetMethods();
	for (size_t i = 0; i < _Methods.size(); ++i)
	{
		os << _Methods[i] << " ";
	}
	os << std::endl;
	os << "Directory Listing: " << (location.GetDirectoryListing() ? "on" : "off") << std::endl;
	os << "Default File: " << location.GetDefaultFile() << std::endl;
	os << "CGI Extensions: ";
	const std::vector<std::string>& cgiExtensions = location.GetCGIextensions();
	for (size_t i = 0; i < cgiExtensions.size(); ++i)
	{
		os << cgiExtensions[i] << " ";
	}
	os << std::endl;
	os << "Redirect: " << location.GetRedirect() << std::endl;
	os << "Directory: " << location.GetDirectory() << std::endl;
	os << "File Upload: ";
	const std::map<std::string, std::string>& fileUpload = location.GetFileUpload();
	std::map<std::string, std::string>::const_iterator iter;
	for (iter = fileUpload.begin(); iter != fileUpload.end(); ++iter)
	{
		os << "\n   Key: " << iter->first << ", Value: " << iter->second;
	}
	std::cout << std::endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, const ServerConfig& server)
{
	os << "Host: " << server.GetHost() << std::endl;
	os << "Port: " << server.GetPort() << std::endl;
	os << "root: " << server.GetRoot() << std::endl;
	os << "Server Names: ";
	const std::vector<std::string>& serverNames = server.GetServerNames();
	for (size_t i = 0; i < serverNames.size(); ++i)
	{
	    os << serverNames[i] << " ";
	}
	os << std::endl;
	os << "Default Server: " << (server.GetDefaultServer() ? "true" : "false") << std::endl;
	os << "Error Pages: ";
	const std::map<int, std::string>& errorPages = server.GetErrorPages();
	std::map<int, std::string>::const_iterator iter;
	for (iter = errorPages.begin(); iter != errorPages.end(); ++iter)
	{
	    os << "Code: " << iter->first << ", Path: " << iter->second << std::endl;
	}
	os << "Limit Client Body Size: " << server.GetLimitClientBodySize() << std::endl;
	os << "Locations: " << std::endl;
	const std::vector<Location>& locations = server.GetLocation();
	for (size_t i = 0; i < locations.size(); ++i)
	{
		os << "   	Location " << (i + 1) << ": " << std::endl;
		os << locations[i];
	}
	return os;
}