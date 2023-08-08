# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 17:30:24 by ullorent          #+#    #+#              #
#    Updated: 2023/08/08 16:35:38 by ecamara          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CPPSRC = src/Server.cpp \
src/Client.cpp \
src/Channel.cpp \
main.cpp \
src/Utils.cpp\
src/ServerData.cpp\
src/Commands.cpp

CPPINCLUDE = include/Server.hpp \
include/Channel.hpp \
include/Client.hpp \
include/defines.h\
include/Utils.hpp\
include/commands.hpp\
include/ServerData.hpp\
include/ServerDataStructs.h\
include/cmd_reply.h\
include/ErrorHandler.hpp\



CPP_FLAGS = -Wall -Werror -Wextra -O2 -std=c++98

ifdef DEBUG
	CPPFLAGS += -DDEBUG_MODE -g3 -fsanitize=address
endif

ifdef LOG
	CPPFLAGS += -DLOG_MODE
endif

ifdef MINISHELL
	CPPFLAGS += -DMINISHELL_MODE
endif

CPPOBJ = $(CPPSRC:.cpp=.o)

CC = clang++

all: $(NAME)

%.o: %.cpp $(CPPINCLUDE)
	@$(CC) $(CPPFLAGS) -c $< -o $@

$(NAME): $(CPPOBJ) $(CPPINCLUDE)
	@$(CC) $(CPPFLAGS) $(CPPOBJ) -I. -o $(NAME)
	@echo "\033[92mircserv has been successfully compiled!\033[0m"



clean:
	@rm -rf $(CPPOBJ)
	@rm -rf src/*.dSYM

fclean: clean
	@rm -rf $(NAME)
	@echo "\033[33mAll has been cleaned!\033[0m"

re: fclean all

.PHONY: all clean fclean re