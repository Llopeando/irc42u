# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/19 13:20:51 by ecamara           #+#    #+#              #
#    Updated: 2023/05/22 13:28:58 by ecamara          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang++
CPPFLAGS = -std=c++17 -Wall -Werror -Wextra -O2 -g# -g3 -fsanitize=address

SRC_FILES =	src/HandShakeServer.cpp\
			src/UdpClient.cpp\
			src/ThreadedServer.cpp\
			src/ServerData.cpp\
			src/UdpServer.cpp

OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Libraries
INCLUDES = -I./libraries
#LIBS = -L./../libraries/mlx -lmlx -framework OpenGL -framework AppKit
#INCLUDES = -I/usr/local/include -I/Users/ecamara/.brew/include

STATIC_LIB = UdpConnection.a

all: $(STATIC_LIB)

$(STATIC_LIB): $(OBJ_FILES)
	ar rcs $(STATIC_LIB) $(OBJ_FILES)
%.o: %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $^ -o $@

clean:
	rm -f $(OBJ_FILES) $(NAME)

re: clean all

.PHONY: all clean re