/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkarib <gkarib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 10:47:52 by gkarib            #+#    #+#             */
/*   Updated: 2023/07/20 19:03:04 by gkarib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/WebServerIncludes.hpp"

Location::Location()
{
}

Location::~Location()
{
}

Location ::Location(const Location& other)
{
	*this = other;
}

Location&	Location::operator=(const Location& other)
{
	if (this != &other)
	{
		this->_Path = other._Path;
		this->_Root = other._Root;
		this->_Index = other._Index;
		this->_Methods = other._Methods;
		this->_CgiExtensions = other._CgiExtensions ;
		this->_FileUpload = other._FileUpload;
		this->_Redirect = other._Redirect;
		this->_Directory = other._Directory;
		this->_DirectoryListing = other._DirectoryListing;
		this->_DefaultFile = other._DefaultFile;
	}
	return (*this);
}

// --------------------------------- GETTERS ---------------------------------

std::string	Location::GetPath( void ) const
{
	return(this->_Path);
}

std::string	Location::GetRoot( void ) const
{
	return(this->_Root);
}

std::string	Location::GetIndex( void ) const
{
	return(this->_Index);
}

std::vector<std::string>	Location::GetMethods( void ) const
{
	return(this->_Methods);
}

bool	Location::GetDirectoryListing( void ) const
{
	return(this->_DirectoryListing);
}

std::string	Location::GetDefaultFile( void ) const
{
	return(this->_DefaultFile);
}

std::vector<std::string>	Location::GetCGIextensions( void ) const
{
	return(this->_CgiExtensions);
}

std::map<std::string, std::string>	Location::GetFileUpload( void ) const
{
	return (this->_FileUpload);
}

std::string	Location::GetRedirect( void ) const
{
	return(this->_Redirect);
}

std::string	Location::GetDirectory( void ) const
{
	return(this->_Directory);
}

// --------------------------------- SETTERS ---------------------------------

void	Location::SetPath(const std::string& path)
{
	this->_Path = path;
}

void	Location::SetRoot(const std::string& root)
{
	this->_Root = root;
}

void	Location::SetIndex(const std::string& index)
{
	this->_Index = index;
}

void	Location::SetMethods(const std::vector<std::string>& methods)
{
	this->_Methods = methods;
}

void	Location::SetDirectoryListing(bool _DirectoryListing)
{
	this->_DirectoryListing = _DirectoryListing;
}

void	Location::SetDefaultFile(const std::string& defaultFile)
{
	this->_DefaultFile = defaultFile;
}

void	Location::SetCGIextensions(const std::vector<std::string>& CgiExtensions)
{
	this->_CgiExtensions = CgiExtensions;
}

void	Location::SetFileUpload(const std::map<std::string, std::string>& fileUpload)
{
	this->_FileUpload = fileUpload;
}

void Location::SetRedirect(const std::string& redirect)
{
	this->_Redirect = redirect;
}

void	Location::SetDirectory(const std::string& _Directory)
{
	this->_Directory = _Directory;
}

// --------------------------------- OTHER FUNCTIONS ---------------------------------

std::vector<std::string>	Location::Parse_array(std::string method_value)
{
	std::vector<std::string>	methods;
	size_t openingBracketPos = method_value.find('[');
	size_t closingBracketPos = method_value.find(']');
	size_t pos = 0;


	if (openingBracketPos != std::string::npos && closingBracketPos != std::string::npos) 
	{
		std::string	array_method = method_value.substr(openingBracketPos + 1, closingBracketPos - 1);
		
		size_t	found = array_method.find(',', pos);
		while (found != std::string::npos)
		{
			methods.push_back(trim_white_spaces(array_method.substr(pos, found - pos)));
			pos = found + 1;
			found = array_method.find(',', pos);
		}
		methods.push_back(trim_white_spaces(array_method.substr(pos)));
	}

	return (methods);
}

void	Location::GetPath_location(const std::string& line)
{
	std::string path;
	path = line.substr(line.find("location") + strlen("location"));
	path = trim_white_spaces(path);
	path = path.substr(0, path.find_first_of(' '));
	this->SetPath(path);
}


void	Location::fill_locationInfos(std::ifstream &config_file, const std::string& line)
{
		(void) line;
		std::string	location_line; // why should i declare here another string to read and not use line from the param ?

		// std::cout << line << std::endl;
		GetPath_location(line);
		while(getline(config_file, location_line))
		{
			// std::istringstream iss(location_line);
			// std::string key;

			// iss >> key;
			// iss >> __DirectoryListing;

			location_line = isComment(location_line);
			location_line = trim_white_spaces(location_line);

			// std::cout << location_line << std::endl;
			// test_brace(location_line);
			// std::cout << location_line << std::endl;
			if (location_line == "}")
				break ;

			if (location_line.find("methods") != std::string::npos)
			{
				std::string	method_value = location_line.substr(location_line.find(":") + 1);
				method_value = trim_white_spaces(method_value);
				this->_Methods = Parse_array(method_value);
				// std::cout << "_Methods->";
				// for (size_t i = 0; i < _Methods.size(); ++i)
				// {
				// 	std::cout << _Methods[i] << "-";
				// 	if (i == _Methods.size() - 1)
				// 		std::cout << std::endl;
				// }
			}
			if (location_line.find("directoryListing") != std::string::npos)
			{
				if (location_line.find(":") != std::string::npos)
				{
					std::string	val = trim_white_spaces(location_line.substr(location_line.find(":") + 1));

					if (val == "on")
						this->_DirectoryListing = true;
					else if (val == "off")
						this->_DirectoryListing = false;
					// std::cout << "__DirectoryListing->" << _DirectoryListing <<std::endl;
				}
			}
			if (location_line.find("default_file") != std::string::npos)
			{
				if (location_line.find(":") != std::string::npos)
					this->_DefaultFile = trim_white_spaces(location_line.substr(location_line.find(":") + 1));
				// std::cout << "default_file->" << this->_DefaultFile << std::endl; 
			}
			if (location_line.find("root") != std::string::npos)
			{
				// TODO: if ':' is not found
				if (location_line.find(":") != std::string::npos)
					this->_Root = trim_white_spaces(location_line.substr(location_line.find(":") + 1));
				// std::cout << "default_file->" << this->_DefaultFile << std::endl; 
			}
			if (location_line.find("index:") != std::string::npos)
			{
				// TODO: if ':' is not found
				if (location_line.find(":") != std::string::npos)
					this->_Index = trim_white_spaces(location_line.substr(location_line.find(":") + 1));
				// std::cout << "default_file->" << this->_DefaultFile << std::endl; 
			}
			if (location_line.find("cgi_extensions") != std::string::npos)
			{
				std::string	_CgiExtensions_array = location_line.substr(location_line.find(":") + 1);
				_CgiExtensions_array = trim_white_spaces(_CgiExtensions_array);
				this->_CgiExtensions = Parse_array(_CgiExtensions_array);
				// std::cout << "_CgiExtensions->";
				// for (size_t i = 0; i < _CgiExtensions.size(); ++i)
				// {
				// 	std::cout << _CgiExtensions[i] << "-";
				// 	if (i == _CgiExtensions.size() - 1)
				// 		std::cout << std::endl;
				// }
			}
			if (location_line.find("redirect") != std::string::npos)
			{
				if (location_line.find(":") != std::string::npos)
				{
					this->_Redirect = trim_white_spaces(location_line.substr(location_line.find(":") + 1));

					// std::cout << "_Redirect->" << _Redirect <<std::endl;
				}
			}
			if (location_line.find("directory") != std::string::npos && location_line.find("DirectoryListing"))
			{
				if (location_line.find(":") != std::string::npos)
				{
					this->_Directory = trim_white_spaces(location_line.substr(location_line.find(":") + 1));
					// std::cout << "_Directory->" << _Directory <<std::endl;
				}
			}
			if (location_line.find("file_upload") != std::string::npos)
			{
				std::string _FileUpload_line;

				while (getline(config_file, _FileUpload_line))
				{
					_FileUpload_line = isComment(_FileUpload_line);
					_FileUpload_line = trim_white_spaces(_FileUpload_line);
					
					if (_FileUpload_line == "}")
					{
						// test_brace(_FileUpload_line);
						break;
					}
		
					// size_t found = error_line.find(":");
					if (_FileUpload_line.find(":") != std::string::npos)
					{
						std::string key = _FileUpload_line.substr(0, _FileUpload_line.find(":"));
						std::string value = _FileUpload_line.substr(_FileUpload_line.find(":") + 1);
						
						this->_FileUpload[key] = trim_white_spaces(value);
					}
			
				}
				// std::cout << "_FileUpload=>";
				// std::map<std::string, std::string>::const_iterator iter;
				// for (iter = _FileUpload.begin(); iter != _FileUpload.end(); ++iter)
				// 	std::cout << "_FileUpload first->" << iter->first << ", _FileUpload second->" << iter->second << std::endl;
				// std::cout << std::endl;
			}
		}
}
