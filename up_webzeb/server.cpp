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
	int	opts = 0;
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
	// pars			p; //this is the struct i addded to store parsing infos;
	int 			r_val;
	char			r_buff[1024] = {0};
	struct pollfd	fd_poll[c_num];
	int				nfds;

	fd_poll[0].fd = serverSocket;
	fd_poll[0].events = POLLIN;
	while (1) {
		std::cout << "\n---------------- Waiting for a new connection ----------------\n";
		if ((nfds = poll(fd_poll, c_num, -1)) < 0)
			throw std::runtime_error(strerror(errno));
		if ((fd_poll[0].revents & POLLIN) && (clientSocket = accept(serverSocket, (struct sockaddr*)&address, &addrLength)) < 0)
			throw std::runtime_error(strerror(errno));
		// p.valread = read(clientSocket, r_buff, 1024);
		r_val = read(clientSocket, r_buff, 1024);
		if (r_val < 0)
			throw std::runtime_error("eof reached");
		if (!r_val)
			throw std::runtime_error("connection closed");
		// r_err(p.valread, p);
		std::cout << "\n------------------- New connection accepted -------------------\n";
		std::cout << r_buff << std::endl;
		parseRequest(r_buff);
		std::cout << "\n---------------- connection closed ----------------\n";
		close(clientSocket);
	}
}

void	server::parseRequest(char* buffer) {
	std::string	method, url, httpVersion, line, body;
	std::map<std::string, std::string>	headers;
	std::stringstream ss(buffer);
	if (ss.eof() || ss.tellg() != std::stringstream::pos_type(0))
		throw std::runtime_error("Failed to read the request.");
	// Request line
	getline(ss, method, ' ');
	getline(ss, url, ' ');
	getline(ss, httpVersion, '\r');
	// retrieving the headers
	while (!ss.eof() && ss.tellg() == std::stringstream::pos_type(0)) {
		getline(ss, line, '\r'); // this "line" string is guaranteed to be ending exactly where it should '\r' but
		// is not guaranteed to be starting from exactly the start of the line
		if (line[0] != '\n')
			throw std::runtime_error("Bad request.");
		if (line == "\n\r\n") {
				getline(ss, body);
				break;
		}
		headers[line.substr(1, line.find(":"))] = line.substr(line.find(":") + 1, line.length()); // protect find()
	}
	//if (url length >= (check config))
	if (httpVersion != "HTTP/1.1")
		throw std::runtime_error("Invalid http version.");
	if (method == "GET")
		GET(url, headers);
	else if (method == "POST")
		POST(url, body, headers);
	else if (method == "DELETE")
		DELETE();
	else
		throw std::runtime_error("Invalid method.");
}

std::string	server::getContent(std::string filename) {
	std::ifstream	file(filename.c_str()); // check for more option u can add for a specific opening
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
	// response << "HTTP/1.1 200 OK\nContent-Type: ";
	// response << ("text/" + type + "\nContent-Length: ");
	response << "HTTP/1.1" << status_phrase << "\nContent-Type: ";
	response << type + "\nContent-Length: ";
	response << s.length();
	response << "\n\n";
	response << s;
	// std::cout << response << std::endl;
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

void	server::POST(std::string url, std::string body, std::map<std::string,std::string> headers) {
	(void)url;
	(void)body;
	(void)headers;
}
void	server::DELETE() {/* put ur code here*/}