/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkarib <gkarib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:47:37 by gkarib            #+#    #+#             */
/*   Updated: 2023/07/16 14:42:19 by gkarib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
#define PARSING_HPP

#include "../WebServerIncludes.hpp"

enum
{
	SERVER,
	LOCATION,
	ERROR_PAGES,
	ERROR
};

class   ConfigFile
{
	private:
		std::ifstream				_Config_file;
		std::vector<ServerConfig>	_Servers;

	public:
		ConfigFile();
		~ConfigFile();

		ConfigFile (const ConfigFile& copy);
		ConfigFile& operator=(const ConfigFile& copy);
		
		ConfigFile(std::vector<ServerConfig>& servers) : _Servers(servers) {} // !understand
		

		// --------------------------------- GETTERS ---------------------------------
		
		// std::ifstream					GetConfigFile( void ); // *std::ifstream objects are non-copyable, meaning they cannot be copied or assigned.
		std::vector<ServerConfig> GetServers( void )	const;
		
		// --------------------------------- SETTERS ---------------------------------
		
		void	SetServers(std::vector<ServerConfig> Server);
		
		// --------------------------------- OTHER FUNCTIONS ---------------------------------

		bool	open_file(const char *str);
		bool	check_bracket(std::string line, std::stack<char> &brace, int &close_brace);
		void	read_ConfigFile( void );
		void    Check_Conf_file(const char *str);

};


// ############################	PARSING CONFIG FILE	############################ 

bool		check_extension(const char *str, const char *extension);
std::string	trim_white_spaces(std::string line);
std::string	isComment(std::string line);
int			DefineContext(std::string	line);

#endif