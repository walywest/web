
# pragma once

#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>


void    _post(char *buffer, size_t len, std::string name);
// size_t  ffstrlen(const char *str);