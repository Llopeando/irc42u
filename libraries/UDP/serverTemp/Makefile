# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/18 13:39:42 by ecamara           #+#    #+#              #
#    Updated: 2023/05/19 12:37:02 by ecamara          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang++
CPPFLAGS = -std=c++17 -Wall -Wextra -O2 -g3 -fsanitize=address

SRC_FILES =	mainServer.cpp\
			../src/HandShakeServer.cpp\
			../src/UdpClient.cpp\
			../src/ServerData.cpp\
			../src/UdpServer.cpp\
			../src/ThreadedServer.cpp

OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Libraries
INCLUDES = -I./libraries
#LIBS = -L./../libraries/mlx -lmlx -framework OpenGL -framework AppKit
#INCLUDES = -I/usr/local/include -I/Users/ecamara/.brew/include


NAME = server

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(CPPFLAGS) $(OBJ_FILES) $(INCLUDES) -o $@

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $^ -o $@

clean:
	rm -f $(OBJ_FILES) $(NAME)

re: clean all

.PHONY: all clean re