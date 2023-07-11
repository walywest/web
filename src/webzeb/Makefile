NAME		=	webserv

CC			=	c++

CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98

# CPPFLAGS	+=	-g -fsanitize=address

SRC			=	main.cpp server.cpp pars_post.cpp

OBJ			=	$(SRC:.cpp=.o)

all			:	$(NAME)

COLOR_GREEN := \033[0;32m
COLOR_RESET := \033[0m
BOLD_TEXT := \033[1m
ITALIC_TEXT := \033[3m
BIG_FONT := \033[5m

$(NAME)		:
				@echo "\n\t$(COLOR_GREEN) $(BOLD_TEXT) $(BIG_FONT) $(NAME) $(COLOR_RESET)\n"
				@$(CC) $(CPPFLAGS) $(SRC) -o $(NAME)
				@echo "$(COLOR_GREEN) $(ITALIC_TEXT) \tReady to run! $(COLOR_RESET)\n"

clean		:
				@rm -rf $(OBJ)

fclean		:	clean
				@rm -rf $(NAME)

re			:	fclean all
