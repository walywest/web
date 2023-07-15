#include "server.hpp"

int	ft_strlen(const char* str) {
	int i;
	for (i = 0; str[i] != 0; ++i);
	return i;
}

server::server() {
	startingConnection(AF_INET, SOCK_STREAM, 0, _port); //FIX1 probably would've caused connectivity problem (fixed by replacing address with htonl(INADDR_ANY)); 
	startingServer();
}

server::server(int domain, int type, int protocol ,int port) {
	startingConnection(domain, type, protocol, port);
	startingServer();
}

server::server(server const & obj) {
	*this = obj;
}

server::~server() {}

server&    server::operator=(server const & obj) {
	// serverSocket = obj.serverSocket;
	// connection = obj.connection;
	// addr = obj.addr;
	(void)obj; 
	return *this;
}

int	server::get_sockeFd() {
	return serverSocket;
}

struct sockaddr_in	server::get_address() {
	return address;
}

/*----------------------------------------------- the actual work ------------------------------------------------------*/

void	server::startingConnection(int domain, int type, int protocol, int port) {
	int	opts = 1;
	std::cout << "Starting ..." << std::endl;
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = domain; // AF_INET
	address.sin_port = htons(port); // htons coverts a short integer to a network representation
	address.sin_addr.s_addr = htonl(INADDR_ANY); //FIX1
	memset(address.sin_zero, '\0', sizeof(address.sin_zero));
	addrLength = sizeof(address);
	serverSocket = socket(domain, type, protocol);
	if (serverSocket < 0)
		throw std::runtime_error(strerror(errno));
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof(int)); //FIX2 might fix socket still using port after the end of the programme;
	std::cout << "Bindig ..." << std::endl;
	if (bind(serverSocket, (struct sockaddr*)&address, addrLength) < 0)
		throw std::runtime_error(strerror(errno));
	std::cout << "Listening ..." << std::endl;
	if (listen(serverSocket, backlog) < 0)
		throw std::runtime_error(strerror(errno));
	std::cout << "Success ! ..." << std::endl;
}

void	server::startingServer() {

	char			r_buff[1024] = {0};
	struct pollfd	fd_poll[c_num];
	int				nfds;
	pars			p;//this is the struct i addded to store parsing infos;

	fd_poll[0].fd = serverSocket;
	fd_poll[0].events = POLLIN;
	p.valread = 1;
	while (1)
	{
		std::cout << "\n---------------- Waiting for a new connection ----------------\n";
			fflush(stdout);
		if ((nfds = poll(fd_poll, c_num, -1)) < 0)
			throw std::runtime_error(strerror(errno));
		if ((fd_poll[0].revents & POLLIN) && (clientSocket = accept(serverSocket, (struct sockaddr*)&address, &addrLength)) < 0)
			throw std::runtime_error(strerror(errno));
		std::cout << "\n------------------- New connection accepted -------------------\n";
			fflush(stdout);
		size_t o = 0;
		while (p.valread && p.t_valread <= FILE_SIZE)
		{
			std::cout << "until now read " << o << " bytes" << std::endl;
			std::cout << " but total "<< " bytes = " <<  p.t_valread  << std::endl;
			fflush(stdout);
			p.valread = read(clientSocket, r_buff, 1024);
			std::cout << "has read " << p.valread << std::endl;
			o += p.valread;
			if (p.valread < 0)
				throw std::runtime_error("eof reached");
			if (!p.valread)
				throw std::runtime_error("connection closed");
			std::cout << "parsing" << std::endl;
			fflush(stdout);
			// std::cout << r_buff << std::endl;
			parseRequest(r_buff, p);
		}
		//clearing function should be here to clean p;
		std::cout << "\n---------------- connection closed ----------------\n";
		close(clientSocket);
		p.headers.clear();
	}
}

void	server::parseRequest(char* buffer, pars &p) {
	if (p.headers["method"] == "POST")
		POST(std::string(buffer, p.valread), p);
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
					std::cout << "hahwa reb dl body |" << body << "|" << std::endl;
					std::cout << "\n\nahahaha\n\n\n" << std::endl;
					body.erase(0, 1);
					break;
				}
			}
			else
			{
				std::cout << "line ===> |" << line << "|" << std::endl;
				p.headers[line.substr(0, i)] = line.substr(i + 1, std::string::npos);
			}

		}
		if (p.headers["method"] == "GET")
		{
			std::cout << "Went here" << std::endl;
			throw std::runtime_error("\n\n\n\nhehehe\n");
			GET(p.headers["url"], p.headers);
		}
		else if (p.headers["method"] == "POST")
		{
			std::cout << "went to POST" <<std::endl;
			fflush(stdout);
			std::cout << "this is the body size" << body.size() << std::endl;
			POST(body, p);
		}
		else if (p.headers["method"] == "DELETE")
			DELETE();
		else
		{
			std::cout << "|" << p.headers["method"] << "|" << std::endl;
			throw std::runtime_error("Invalid method.");
		}
	}
}

std::string	server::getContent(std::string filename) {
	std::ifstream	file(filename); // check for more option u can add for a specific opening
	std::cout << " (" << filename << ") " << std::endl;
	if (file.is_open()) {
		std::stringstream 	ss;
		ss << file.rdbuf();
		if (file.fail() || ss.str().empty()) {
			file.close();
			return getContent("../webzeb/errors/404.html");
			// throw std::runtime_error(strerror(errno));
		}
		file.close();
		return ss.str();
	}
	else
		return getContent("../webzeb/errors/404.html");
}

void	server::generateResponse(std::string s, std::string type) {
	std::stringstream	response;
	std::string status_phrase(" 200 OK");
	response << "HTTP/1.1" << status_phrase << "\nContent-Type: ";
	response << type + "\nContent-Length: ";
	response << s.length();
	response << "\n\n";
	response << s;
	if (send(clientSocket, response.str().c_str(), response.str().length(), 0) < 0)
		throw std::runtime_error(strerror(errno));
}

/*----------------------------------------------- methods ------------------------------------------------------*/

void	server::GET(std::string& url,std::map<std::string,std::string> headers) {
	std::string root(test_root), path;
	path = root + url;
	std::cout << " |" << path << "| " << std::endl;
	DIR*  dir = opendir(path.c_str());
	if (dir != NULL) {
		if (!path.empty() && path[path.length() - 1] != '/')
			path += "/";
		// std::cout << path << std::endl;
		struct dirent* entry;
		while ((entry = readdir(dir)) != NULL) {
			std::string tmp(entry->d_name);
			if (tmp == "index.html")
				generateResponse(getContent((path + tmp)), headers["Accept"]);
		}
		closedir(dir);
	}
	else {
		generateResponse(getContent(path), headers["Accept"]);
	}
}

void	server::DELETE() {/* put ur code here*/}