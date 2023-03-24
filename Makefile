# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 17:30:24 by ullorent          #+#    #+#              #
#    Updated: 2023/03/24 21:12:16 by ullorent         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CPPSRC = Server.cpp \
Client.cpp \
Channel.cpp \
main.cpp \

CPPINCLUDE = Server.hpp \
Channel.hpp \
Client.hpp \
irc.h \

CPPOSRC = $(CPPSRC:.cpp=.o)

CPPFLAGS = -Wall -Werror -Wextra
CLANGCMD = c++

all: $(NAME)

%.o: %.cpp
	@$(CLANGCMD) $(CPPFLAGS) -c $^ -o $@

$(NAME): $(CPPOSRC) $(CPPINCLUDE)
	@$(CLANGCMD) $(CPPFLAGS) $(CPPOSRC) -I. -o $(NAME)
	@echo "\033[92mircserv has been successfully compiled!\033[0m"

clean:
	@rm -rf *.o
	@rm -rf *.dSYM

fclean: clean
	@rm -rf $(NAME)
	@echo "\033[33mAll has been cleaned!\033[0m"

re: fclean all

.PHONY: all clean fclean re