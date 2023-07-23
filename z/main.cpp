#include "Headers/WebServerIncludes.hpp"

int main() {
    try {
        // loop in the server struct and for each server intentiate an object of my http server class using the parametrized constructor
        // that will get the default parameters along with the interface and port provided by the server config
        // to do : start working with the information retrieved from the config file, modify the parsing part of the request to handle the request vector, modify the url check and path based on location formulation
       
       
       	std::vector<ServerConfig> servers;
		ConfigFile conf(servers);

		conf.Check_Conf_file("test.conf");


        httpServer test(conf.GetServers());
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}