# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 17:30:24 by ullorent          #+#    #+#              #
#    Updated: 2023/07/17 16:09:41 by ecamara          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CPPSRC = src/Server.cpp \
src/Client.cpp \
src/Channel.cpp \
main.cpp \
src/Utils.cpp\
src/ServerData.cpp\
src/Renderer.cpp\
src/Tile.cpp\


CPPINCLUDE = include/Server.hpp \
include/Channel.hpp \
include/Client.hpp \
include/defines.hpp	\
include/Utils.hpp\
include/cmd_structs.h\
include/commands.h\
include/Config.h\
include/function.h\
include/ServerData.hpp\
include/ServerDataStructs.h\
include/cmd_reply.h

CPPOBJ = $(CPPSRC:.cpp=.o)

CC = clang++
CPPFLAGS = -std=c++17 -Wall -Werror -Wextra -O2 -mavx -mavx2 -march=native -g# -g3 -fsanitize=address 

# Libraries
INCLUDES = -I./libraries
STATIC_MLX = libraries/mlx/libmlx.a
MLX_FLAGS = -framework OpenGL -framework AppKit


all: $(NAME)

$(NAME): $(CPPOBJ)  $(STATIC_MLX)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(STATIC_MLX) $(MLX_FLAGS) $^ -o $@
	@echo "\033[92mircserv has been successfully compiled!\033[0m"

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $^ -o $@

$(STATIC_MLX) :
	@$(MAKE) -C ../libraries/mlx/

$(STATIC_UDP) :
	@$(MAKE) -C ../libraries/UDP/

clean:
	@rm -rf $(CPPOBJ)
	@rm -rf src/*.dSYM

fclean: clean
	@rm -rf $(NAME)
	@echo "\033[33mAll has been cleaned!\033[0m"

re: fclean all

.PHONY: all clean fclean re


