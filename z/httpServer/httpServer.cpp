#include "../Headers/WebServerIncludes.hpp"

httpServer::httpServer() {
	startingConnection("127.0.0.1", _port);
	startingServer();
}

httpServer::httpServer(std::vector <ServerConfig> _servers) : data(_servers)
{
	startingConnection(data[0].GetHost().c_str(), data[0].GetPort());
	startingServer();
}

httpServer::httpServer(httpServer const & obj)
{
	*this = obj;
}

httpServer::~httpServer() {}

// httpServer&    httpServer::operator=(httpServer const & obj) {
// 	// serverSocket = obj.serverSocket;
// 	// connection = obj.connection;
// 	// addr = obj.addr;
// 	(void)obj; 
// 	return *this;
// }

void	httpServer::setStatusPhrase(void) {
	status_phrase[100] = "100 Continue";
	status_phrase[200] = "200 OK";
	status_phrase[202] = "202 Accepted";
	status_phrase[204] = "204 No Content";
	status_phrase[301] = "301 Moved Permanently";
	status_phrase[307] = "307 Temporary Redirect";
	status_phrase[400] = "400 Bad Request";
	status_phrase[401] = "401 Unauthorized";
	status_phrase[403] = "403 Forbidden";
	status_phrase[404] = "404 Not Found";
	status_phrase[405] = "405 Method Not Allowed";
	status_phrase[409] = "409 Conflict";
	status_phrase[411] = "411 Length Required";
	status_phrase[413] = "413 Request Entity Too Large";
	status_phrase[414] = "414 Request-URI Too Large";
	status_phrase[415] = "415 Unsupported Media Type";
	status_phrase[500] = "500 Internal Server Error";
	status_phrase[501] = "501 Not Implemented";
	status_phrase[502] = "502 Bad Gateway";
	status_phrase[503] = "503 Service Unavailable";
	status_phrase[504] = "504 Gateway Time-out";
	status_phrase[505] = "505 HTTP Version not supported";
}

/*----------------------------------------------- the actual work ------------------------------------------------------*/

void	httpServer::startingConnection(const char* interface, int port)
{
	int	opts = 1;
	std::cout << "Starting ..." << std::endl;
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET; // AF_INET
	address.sin_port = htons(port); // htons coverts a short integer to a network representation
	address.sin_addr.s_addr = inet_addr(interface); //FIX1
	memset(address.sin_zero, '\0', sizeof(address.sin_zero));
	addrLength = sizeof(address);
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
		throw std::runtime_error("tooooz");
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof(int)); //FIX2 might fix socket still using port after the end of the programme;
	std::cout << "Bindig ..." << std::endl;
	if (bind(serverSocket, (struct sockaddr*)&address, addrLength) < 0)
		throw std::runtime_error("tooooz");
	std::cout << "Listening ..." << std::endl;
	if (listen(serverSocket, backlog) < 0)
		throw std::runtime_error("tooooz");
	std::cout << "Success ! ..." << std::endl;
}

void	httpServer::startingServer()
{
	int	c_s = 0;
	pars			p;//this is the struct i addded to store parsing infos;

	p.valread = 1;
	while (1)
	{
		char			r_buff[1024] = {0};
		std::cout << "\n---------------- Waiting for a new connection ----------------\n";
			fflush(stdout);
		if ((c_s = accept(serverSocket, (struct sockaddr*)&address, &addrLength)))
			throw std::runtime_error("tooooz");
		std::cout << "\n------------------- New connection accepted -------------------\n";
			fflush(stdout);
		size_t o = 0;
		while ((p.valread && p.end_flag))
		{
			// std::cout << "until now read " << o << " bytes" << std::endl;
			// std::cout << " but total "<< " bytes = " <<  p.t_valread  << std::endl;
			fflush(stdout);
			p.valread = read(c_s, r_buff, 1024);
			// std::cout << "has read " << p.valread << std::endl;
			o += p.valread;
			if (p.valread < 0)
				throw std::runtime_error("eof reached");
			if (!p.valread)
				throw std::runtime_error("connection closed");
			// std::cout << "parsing" << std::endl;
			fflush(stdout);
			// std::cout << r_buff << std::endl;
			parseRequest(r_buff, p, data[0]);
		}
		//clearing function should be here to clean p;
		std::cout << "\n---------------- connection closed ----------------\n";
		close(c_s);
		p.headers.clear();
	}
}

void	httpServer::parseRequest(char* buffer, pars &p, ServerConfig data)
{
	if (p.headers["method"] == "POST")
		POST(std::string(buffer, p.valread), p, data);
	else
	{
		std::string line, body;
		std::string sbuf(buffer, p.valread);
		std::istringstream ss(sbuf);
		size_t i;

		if (ss.eof() || ss.tellg() != std::stringstream::pos_type(0))
			throw std::runtime_error("Failed to read the request.");
		// Request line
		if (!getline(ss, p.headers["method"], ' ') \
		|| !getline(ss, p.headers["url"], ' ') \
		|| !getline(ss, p.headers["httpversion"], '\n'))
			throw std::runtime_error("hahaha");
		//if (url length >= (check config)) + might need to check for allowed characters in the uri;
		if (p.headers["httpversion"] != "HTTP/1.1\r")
		{
			fflush(stdout);
			throw std::runtime_error("Malformed request:: Invalid http version.");
		}
		fflush(stdout);
		while (ss.rdbuf()->in_avail() && !ss.fail()) {
			if (!getline(ss, line, '\r'))
				throw std::runtime_error("hahaha");
			// std::cout << "line ===> |" << line << "|" << std::endl;
			if (line[0] == '\n')
				line.erase(0,1);
			i = line.find(": ");
			if (i == std::string::npos)
			{
				// std::cout << "this is line after the if |"<< line  << "|" << std::endl;
				if (ss.peek() == '\n')
				{
					body = ss.str().substr(ss.tellg());
					// std::cout << "hahwa reb dl body |" << body << "|" << std::endl;
					// std::cout << "size" << body.size() << std::endl;
					// std::cout << "\n\nahahaha\n\n\n" << std::endl;
					body.erase(0, 1);
					break;
				}
			}
			else
			{
				// std::cout << "line ===> |" << line << "|" << std::endl;
				p.headers[line.substr(0, i)] = line.substr(i + 2, std::string::npos);
			}

		}
		if (p.headers["method"] == "GET")
		{
			// std::cout << "Went here" << std::endl;
			throw std::runtime_error("\n\n\n\nhehehe\n");
			GET(p.headers["url"], p.headers, getMatchingLocation(p.headers["url"], p.headers["method"], data.GetLocation()), data);
		}
		else if (p.headers["method"] == "POST")
		{
			std::cout << "went to POST" <<std::endl;
			fflush(stdout);
			// std::cout << "this is the body size" << body.size() << std::endl;
			POST(body, p, data);
		}
		else if (p.headers["method"] == "DELETE")
			DELETE(p.headers["url"], p.headers, getMatchingLocation(p.headers["url"], p.headers["method"], data.GetLocation()), data);
		else
		{
			std::cout << "|" << p.headers["method"] << "|" << std::endl;
			throw std::runtime_error("Invalid method.");
		}
	}
}


void	httpServer::is_valid(std::map<std::string,std::string> headers, std::string method, std::string url, std::string httpVersion) {
	if (httpVersion != "HTTP/1.1")
		status =  400;
	if (method != "GET" && method != "POST" && method != "DELETE")
		status =  405;
	if (headers["Transfer-Encoding"].find("chunked") == std::string::npos)
		status =  501;
	if (method == "POST" && headers.find("Transfer-Encoding") == headers.end() && headers.find("Content-Lenght") == headers.end())
		status =  400;
	if (url.length() > 2048)
		status =  414;
	if (url.find("{}[]\n\r\t\v\f\\\"\'<>~") != std::string::npos)
		status =  400;
	// if body is larger than the max body size return 413
}

const Location*	httpServer::getMatchingLocation(std::string url, std::string method, const std::vector<Location>& location) {
	size_t i;
	// Iterate through the available Location objects
	for (i = 0; i < location.size(); ++i) {
    	// Compare the requested URL with the defined path in the Location object
		std::string path = location[i].GetPath();
    	if (url.find(path) == 0 && url.substr(path.length(), url.length())[0] == '/') { // check if equal
        	// Check if the HTTP method is allowed for the matched Location object
        	std::vector<std::string> allowedMethods = location[i].GetMethods();
        	if (std::find(allowedMethods.begin(), allowedMethods.end(), method) != allowedMethods.end()) {
            	// Additional checks on directory listing, CGI extensions redirections
            	// if everything good get the first matched location
            	return &(location[i]);
        	}
			else
				status = 405; // method not allowed
			break;
    	}
	}
	if (i == location.size())
		status = 404; // no matched found
	return NULL;
	// Process the request based on the matched Location object and its configuration
    // No matching Location found, handle accordingly (e.g., return a 404 Not Found response)
    // Matched Location found, process the request using the information and configuration from the matched Location
}

std::string	httpServer::getRoot(Location rootLocation, std::string serverRoot, Location matchedLocation) {
	// will need the location matched + the server corresponding to the response + the root location
	// get the correct root :
	// if theres no root specified in the location block look for 
	//the the root location block if does not exist either then use th eroot specified in the server block
	// if no root is specified in server block then redirect to 404 error page
	// if rootlocation.GetPath() == "/"
	if (!matchedLocation.GetRoot().empty())
		return (matchedLocation.GetRoot());
	else if (!rootLocation.GetRoot().empty())
		return (rootLocation.GetRoot());
	else if (!serverRoot.empty())
		return (serverRoot);
	else
		return ("");

}

std::vector<std::string> httpServer::getDirContent(std::string path) {
	std::vector<std::string> dirContent;
	DIR* dir = opendir(path.c_str());
	if (dir == NULL) {
		return dirContent;
	}
	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) {
		dirContent.push_back(entry->d_name);
	}
	closedir(dir);
	return dirContent;
}

std::string	httpServer::generateDirectoryListing(const std::vector<std::string>& dirContent) {
	std::string Content;

    std::string htmlContent("<html><head><title>Directory Listing</title></head><body><h1>Directory Listing</h1><ul>");
	for (size_t i = 0; i < dirContent.size(); ++i) {
        if (dirContent[i] != "." && dirContent[i] != "..") {
            htmlContent += "<li><a href=\"" + dirContent[i] + "\">" + dirContent[i] + "</a></li>";
        }
	}
    htmlContent += "</ul></body></html>";
	return htmlContent;
}

std::string	httpServer::getPath(std::string url, const Location* matchedLocation, std::string root) {
	if (root.empty() || !matchedLocation) {
		status = 404;
		return "../errors/404.html";
	}
	std::string path;
    // handle the root url and remove trailing slashes
    if (url == "/")
		path = root;
	else {
        while (!url.empty() && url.back() == '/') {
            url.pop_back();
        }
		if (!matchedLocation->GetRedirect().empty()) {
			status = 301; // or 302
			path =  root + matchedLocation->GetRedirect();
			// set the Location header to the new URL
		} else
			path = root + url;
    }
    // handle the case when the path points to a directory
	std::cout << " |" << path << "| " << std::endl;
	std::vector<std::string> dirContent = getDirContent(path); // check if its empty
	if (dirContent.size() != 0) {
		path += "/";
		// std::cout << path << std::endl;
		if (!matchedLocation->GetIndex().empty()) {
			if (std::find(dirContent.begin(), dirContent.end(), matchedLocation->GetIndex()) != dirContent.end()) {
				status = 200;
				return path + matchedLocation->GetIndex();
			}
			else {
				status = 404; // wla maert 403
				return "../errors/404.html";
			}
		}
		if (matchedLocation->GetDirectoryListing()) {
			status = 200;
			return path;
		} else {
			status = 403;
			return "../errors/403.html";
		}		
	}
	// handle the case when the path points to a file
	return (path);
}

std::string	httpServer::getContent(std::string filename) {
	std::ifstream	file(filename.c_str()); // check for more option u can add for a specific opening
	std::cout << " (" << filename << ") " << std::endl;
	if (file.is_open()) {
		std::stringstream 	ss;
		ss << file.rdbuf();
		if (file.fail() || ss.str().empty()) {
			file.close();
			status = 404;
			return getContent("../errors/404.html");
			// throw std::runtime_error(strerror(errno));
		}
		file.close();
		return ss.str();
	}
	else {
		status = 404;
		return getContent("../errors/404.html");
	}
		
}

// modify the generation of response to handle heavier files by sending them in chunks
void	httpServer::generateResponse(std::string s, std::string type) {
	(void)s;
	(void)type;
	// size_t					c_num = data.size();
	// std::stringstream	response[c_num];
	// for (size_t i = 0; i < c_num; ++i) {
	// 	response[i] << ("HTTP/1.1" + status_phrase[status] + "\nContent-Type: ");
	// 	// response[i] << ("HTTP/1.1 200 OK \nContent-Type: ");
	// 	response[i] << type + "\nContent-Length: ";
	// 	response[i] << s.length();
	// 	response[i] << "\n\n";
	// 	response[i] << s;
	// 	// std::cout << "hadi response :\n(" + response[i].str() + ")" << std::endl;
	// 	if (send(clientSocket[i], response[i].str().c_str(), response[i].str().length(), 0) < 0)
	// 		throw std::runtime_error(strerror(errno));
	// 	close(clientSocket[i]);
	// }

}

/*----------------------------------------------- methods ------------------------------------------------------*/

void	httpServer::GET(std::string& url, std::map<std::string,std::string> headers, const Location* matchedLocation, ServerConfig data) {
	std::string path = getPath(url, matchedLocation, getRoot(data.GetLocation()[0], data.GetRoot(), *matchedLocation));
    if (status == 200 && matchedLocation->GetDirectoryListing())
		generateResponse(generateDirectoryListing(getDirContent(path)), headers["Content-Type"]);
	else if (matchedLocation->GetCGIextensions().size() != 0
		&& std::find(matchedLocation->GetCGIextensions().begin(), matchedLocation->GetCGIextensions().end(), headers["Content-Type"]) != matchedLocation->GetCGIextensions().end()) {
       // add an extention check on the file stored in path
	   // Execute the CGI script and generate the response
        // generateResponse(CGI(query, path, env), headers["Content-Type"]);
    } else
        generateResponse(getContent(path), headers["Content-Type"]);
}

void	httpServer::DELETE(std::string& url, std::map<std::string,std::string> headers, const Location* matchedLocation, ServerConfig data) {
	// modify getPath to start sending the path found directly instead of calling getContent
	std::string path = getPath(url, matchedLocation, getRoot(data.GetLocation()[0], data.GetRoot(), *matchedLocation));
	std::vector<std::string> dirContent = getDirContent(path);
	std::string	s;
	if (dirContent.size() != 0) {
		for (size_t i = 0; i < dirContent.size(); ++i) {
			if (std::remove(dirContent[i].c_str()) == 0)
				status = 200;
			else
				status = 500;
				return ;
		}
	}
	else {
		if (std::remove(path.c_str()) == 0)
			status = 200;
		else
			status = 404;		
	}
	generateResponse(s, headers["Content-Type"]);
}

// void	httpServer::CGI(std::string url, char** env) {

// }