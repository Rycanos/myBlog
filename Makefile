# Compiler
CXX =		g++

# Flags
CFLAGS =	-lstdc++ -Wall -Werror -g

# Source files
SRC =		src/server_linux.cpp src/http_tcpServer_linux.cpp

# Include folder
INCLUDES =	-I./include/

# Name of output
NAME =		HttpLinux

# Source files to object files
OBJS =		$(SRC:.cpp=.o)

# 'make all' command
all:		$(NAME)

# Compile
$(NAME):	$(OBJS)
		$(CXX) $(INCLUDES) -o $(NAME) $(OBJS) $(CFLAGS)

# Deletes object (.o) files
clean:
		rm -rf $(OBJS)

# Deletes object files and binary
fclean:		clean
		rm -rf $(NAME)

# Deletes all and recompiles
re:		fclean all

.PHONY:		all clean fclean re
