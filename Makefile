# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alyasar <alyasar@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/25 20:55:10 by altugyasar        #+#    #+#              #
#    Updated: 2023/03/05 19:25:45 by alyasar          ###   ########.tr        #
#                                                                              #
# **************************************************************************** #

RM		=	rm -rf
CC		=	c++
STD		=	-std=c++98
NAME	=	ircserv
FLAGS	=	-Wall -Werror -Wextra

SOURCES	=	./src/main.cpp \
			./src/User.cpp \
			./src/Server.cpp \
			./src/Channel.cpp \
			./src/Utils.cpp \
			./src/Command.cpp

OBJECTS	= $(SOURCES:.cpp=.o)

$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(STD) -o $(NAME)

all: $(NAME)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re