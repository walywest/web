/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkarib <gkarib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 21:01:35 by gkarib            #+#    #+#             */
/*   Updated: 2023/07/16 09:57:56 by gkarib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/WebServerIncludes.hpp"

bool    check_extension(const char *str, const char *extension)
{
	if (strlen(str) > strlen(extension))
	{
		if (!strncmp(&str[strlen(str) - strlen(extension)], extension, strlen(extension)))
			return (true);
	}
	std::cerr << "Error: Invalid Extension." << std::endl;
	return (false);
}

bool	ConfigFile::open_file(const char *file_name)
{
	_Config_file.open(file_name);

	if (!_Config_file)
	{
		std::cerr << "Error : Can't Open File." << std::endl;
		return (false);
	}
	if (_Config_file.peek() == EOF)
	{
		std::cerr << "Error: Empty File." << std::endl;
		_Config_file.close();
		return (false);
	}
	return (true);
}

std::string	trim_white_spaces(std::string line)
{
	int	i;
	int	j;

	i = 0;
	j = line.length() - 1;
	while(isspace(line[i]))
		i++;
	while(isspace(line[j]))
		j--;
	return(line.substr(i, j - i + 1));
}

std::string	isComment(std::string line)
{
	size_t found1 = line.find('#');
	size_t found2 = line.find("//");

	if ((found1 != std::string::npos && found2 == std::string::npos) || found1 < found2)
		return (line.substr(0, found1));
	else if (found2 != std::string::npos)
		return (line.substr(0, found2));
	return (line);
}


bool	ConfigFile::check_bracket(std::string line, std::stack<char> &brace, int &close_brace)
{
	// std::cout << line << std::endl;
	for (size_t i = 0; i < line.length(); i++)
	{

		if (line[i] == '{')
		{
			// std::cout << "************************** pushed **************************" <<std::endl;
			// brace.push(line[i]);
			if (DefineContext(line) == LOCATION)
			{
				// std::cout << line << std::endl;
				// std::cout << "************************** poped **************************" <<std::endl;
				brace.pop();
			}
			if (DefineContext(line) == ERROR_PAGES)
			{
				// std::cout << line << std::endl;
				// std::cout << "************************** poped **************************" <<std::endl;
				brace.pop();
			}
		}
		else if (line[i] == '}')
		{
			if (brace.empty())
			{
				close_brace++;
				// std::cout << "************************** keeping track of a close brace **************************" <<std::endl;
				return (false);
			}
			else
				// std::cout << "************************** poped **************************" <<std::endl;
				brace.pop();
		}	
	}
	if (!brace.empty())
	{
		// std::cout << "Error: brace '{'" << std::endl;
		return (false);
	}
	return (true);
}


int	DefineContext(std::string	line)
{
	if (line == "server {")
		return (SERVER);
	if (line == "error_pages: {")
		return (ERROR_PAGES);
	else if (!line.compare(0, 9, "location "))
		return (LOCATION);
	return (ERROR);
}

// std::vector<char>	test_brace(std::string line)
// {
// 	std::vector<char>	brace;
	
// 	for (size_t i = 0; i < line.length(); i++)
// 	{
// 		if (line[i] == '{')
// 		{
// 			std::cout << "loc : ---------------->" << line << std::endl;
// 			brace.push_back(line[i]);
// 		}
// 		else if (line[i] == '}')
// 		{
// 			std::cout << "loc : ---------------->" << line << std::endl;
// 			brace.push_back(line[i]);
// 		}
// 	}
// 	std::cout << "inside the vec =>" << std::endl; 
// 	for (size_t i = 0; i < brace.size(); i++)
// 		std::cout << brace[i] << " ";
// 	std::cout << std::endl;
// 	return (brace);
// }
