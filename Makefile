# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 17:30:24 by ullorent          #+#    #+#              #
#    Updated: 2023/07/11 14:06:40 by ullorent         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CPPSRC = src/Server.cpp \
src/Client.cpp \
src/Channel.cpp \
main.cpp \
src/Utils.cpp\
src/ServerData.cpp

CPPINCLUDE = include/Server.hpp \
include/Channel.hpp \
include/Client.hpp \
include/defines.hpp	\
include/Utils.hpp\
include/command_structs.h\
include/commands.h\
include/command_structs.h\
include/Config.h\
include/function.h\
include/ServerData.hpp\
include/ServerDataStructs.h

CPPOBJ = $(CPPSRC:.cpp=.o)

CPPFLAGS = -Wall -Werror -Wextra -O2 -std=c++98 #-g3 -fsanitize=address
CC = clang++

all: $(NAME)

%.o: %.cpp $(CPPINCLUDE)
	@$(CC) $(CPPFLAGS) -c $< -o $@

$(NAME): $(CPPOBJ) $(CPPINCLUDE)
	@$(CC) $(CPPFLAGS) $(CPPOBJ) -I. -o $(NAME)
	@echo "\033[92mircserv has been successfully compiled!\033[0m"

clean:
	@rm -rf src/*.o
	@rm -rf src/*.dSYM

fclean: clean
	@rm -rf $(NAME)
	@echo "\033[33mAll has been cleaned!\033[0m"

re: fclean all

.PHONY: all clean fclean re