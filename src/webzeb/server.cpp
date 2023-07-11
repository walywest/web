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

int	server::pre_parse(pars& p)
{
	char	buffer[1024] = {0};
	p.valread = read(clientSocket, buffer, 1024);
	std::cout << buffer << std::endl; //might not be null-terminated POTENTIAL SEG_FAULT;
	std::string buff(buffer, (size_t)p.valread);
	if (p.valread != -1 && p.valread)
	{
		if (buff.find("POST") == 0)
		{
			while (p.p_h == 1)
			{
				p.upd_valread();
				p.header += std::string(buffer, (size_t)p.valread);
				p.valread = read(clientSocket, buffer, 1024);
				r_err(p.valread);
				split_head_body(buffer, &p);
			}
			return (2);
		}
		else
		{
			p.upd_valread();
			return (1);
		}
	}
	else if (p.valread != -1)
	{
        perror("error reading from socket");
		throw std::runtime_error(strerror(errno));
	}
	else
	{
        perror("server hanging while reading form socket");
		throw std::runtime_error(strerror(errno));
	}
	exit(0);
}

void	server::startingServer() {
	pars p;
	while (1) {
		std::cout << "\n---------------- Waiting for a new connection ----------------\n";
		if ((clientSocket = accept(serverSocket, (struct sockaddr*)&address, &addrLength)) < 0)
			throw std::runtime_error(strerror(errno));
		std::cout << "\n------------------- New connection accepted -------------------\n";
		// NOTE2 since starting the server is a common task i will be making changes to this function to call ur parsing if i don't find POST in the initial buffer
		//as well as Ghita also will be making simple but important changes for the multiplexing;
		//pre-parsing function ===>
		if (!p.type)
			p.type = pre_parse(p); //this would call the right parsing for the rest of the reading
		switch (p.type)
		{
			case 1:
				post_parse(p);
				break;	
			case 2:
			{

			// 	//**** associate with number 2;
			char buffer[1024] = {0};
			p.valread = read(clientSocket, buffer, 1024);
			r_err(p.valread);
				parseRequest(buffer);
				std::cout << "\n---------------- connection closed ----------------\n";
				close(clientSocket);
			// 	//*****
				break;	
			}
			default:
				break;
		}
	}
}

void	server::parseRequest(char* buffer) {
	std::string	method, url, httpVersion;
	std::stringstream ss(buffer);
	// retrieve the following lines from the request and save them in a container
	// Host for GET, Content-type for post and the Content-Length for POST and DELETE
	//NOTE1 MIGHT NEED HOST HEADER FOR ALL of the 3 METHODS
	while (!ss.eof() && ss.tellg() == std::stringstream::pos_type(0)) {
		getline(ss, method, ' ');
		getline(ss, url, ' ');
		getline(ss, httpVersion, '\r');
		//if (url length >= (check config))
		if (httpVersion != "HTTP/1.1")
			throw std::runtime_error("Invalid http version.");
		if (method == "GET")
			GET(url);
		else
			throw std::runtime_error("Invalid method.");
	}
}

std::string	server::getContent(std::string filename) {
	std::ifstream	file(filename); // check for more option u can add for a specific opening
	std::cout << filename << std::endl;
	if (file.is_open()) {
		std::stringstream 	ss;
		ss << file.rdbuf();
		if (file.fail() || ss.str().empty()) {
			file.close();
			throw std::runtime_error(strerror(errno));
		}
		file.close();
		return ss.str();
	}
	else
		throw std::runtime_error(strerror(errno));
}

void	server::generateResponse(std::string s, std::string type) {
	std::stringstream	response;
	response << "HTTP/1.1 200 OK\nContent-Type: ";
	response << ("text/" + type + "\nContent-Length: ");
	response << s.length();
	response << "\n\n";
	response << s;
	std::cout << response << std::endl;
	if (send(clientSocket, response.str().c_str(), response.str().length(), 0) < 0)
		throw std::runtime_error(strerror(errno));
}

/*----------------------------------------------- methods ------------------------------------------------------*/

void	server::GET(std::string& url) {
	std::string root(test_root), path;
	path = root + url;
	DIR*  dir = opendir(path.c_str());
	if (dir != NULL) {
		if (path.back() != '/')
			path += "/";
		std::cout << path << std::endl;
		struct dirent* entry;
		while ((entry = readdir(dir)) != NULL) {
			std::string tmp(entry->d_name);
			if (tmp == "index.html")
				generateResponse(getContent((path + tmp)), "html");
		}
		closedir(dir);
	}
	else {
		size_t i = url.find(".");
		if (i != std::string::npos)
			generateResponse(getContent(path), url.substr(i + 1));
		generateResponse(getContent(path), "plain");	
	}
}

