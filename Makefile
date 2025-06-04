# Compiler
CXX =		gcc

# Flags
CFLAGS =	-lstdc++ -Wall -g

# Source files
SRC =		server_linux.cpp http_tcpServer_linux.cpp

# Name of output
NAME =		HttpLinux

# Source files to object files
OBJS =		$(SRC:.cpp=.o)

# 'make all' command
all:		$(NAME)

# Compile
$(NAME):	$(OBJS)
		$(CXX) -I. -o $(NAME) $(OBJS) $(CFLAGS)

# Deletes object (.o) files
clean:
		rm -rf $(OBJS)

# Deletes object files and binary
fclean:		clean
		rm -rf $(NAME)

# Deletes all and recompiles
re:		fclean all
