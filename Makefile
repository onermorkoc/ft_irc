RM		=	rm -rf
CC		=	c++
STD		=	-std=c++98
NAME	=	ircserv
FLAGS	=	-Wall -Werror -Wextra

SOURCES	=	./Sources/main.cpp \
			./Sources/Error.cpp \
			./Sources/Server.cpp \
			./Sources/Utils.cpp

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