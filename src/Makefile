FLAGS=-Wall -Wextra -Werror -std=c++98 -fsanitize=address -g
NAME=webserver

SRC=main.cpp\

OBJ=$(SRC:.cpp=.o)

all:$(NAME)

$(NAME):$(OBJ)
	c++ $(FLAGS) -o $@ $^

$(OBJ):$(SRC)
	c++ $(FLAGS) -c $^

clean:
	rm -rf $(OBJ);

fclean:clean
	rm -rf $(NAME)

re:fclean all
