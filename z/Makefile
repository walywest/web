NAME		=	webserv

CC			=	g++

CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98

# CPPFLAGS	+=	-g -fsanitize=address

INCLUDES	=	-I ./Headers

SRC			=	main.cpp httpServer/httpServer.cpp httpServer/post.cpp

PARSING		=	Parsing/config_file.cpp \
				Parsing/ServerConfig.cpp \
				Parsing/Location.cpp \
				Parsing/Utils.cpp \
				Parsing/overload_operator.cpp

OBJ			=	$(SRC:.cpp=.o) $(PARSING:.cpp=.o)

all			:	$(NAME)

COLOR_GREEN := \033[0;32m
COLOR_RESET := \033[0m
BOLD_TEXT := \033[1m
ITALIC_TEXT := \033[3m
BIG_FONT := \033[5m

$(NAME)		:
				@echo "\n\t$(COLOR_GREEN) $(BOLD_TEXT) $(BIG_FONT) $(NAME) $(COLOR_RESET)\n"
				@$(CC) $(CPPFLAGS) $(PARSING) $(SRC) $(INCLUDES) -o $(NAME)
				@echo "$(COLOR_GREEN) $(ITALIC_TEXT) \tReady to run! $(COLOR_RESET)\n"

clean		:
				@rm -rf $(OBJ)

fclean		:	clean
				@rm -rf $(NAME)

re			:	fclean all