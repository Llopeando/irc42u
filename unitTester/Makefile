NAME = unitTester

SRC =	mainTester.cpp\
		../src/Utils.cpp

OBJ  = $(SRC:.cpp=.o)

INCLUDES =	Checker.hpp\
			../include/Utils.hpp

FLAGS = -Wall -Werror -Wextra -std=c++17 -g3 -fsanitize=address

CC = clang++


all: $(NAME)

%.o: %.cpp
	$(CC) $(FLAGS) -c $^ -o $@

$(NAME): $(OBJ) $(INCLUDES)
	$(CC) $(FLAGS) $(OBJ) -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

PHONY: all re clean fclean