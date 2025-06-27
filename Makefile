# Variables
NAME	=	pipex
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror
RM		=	rm -f

SRC = pipex.c \
	  utils.c \

OBJ = $(SRC:.c=.o)
HEADERS = pipex.h

LIBDIR = libft
LIB = $(LIBDIR)/libft.a

# Target principal
all: $(LIB) $(NAME)

# Regla para compilar ejecutable, enlazando la librería
$(NAME): $(OBJ) $(LIB)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBDIR) -lft -o $(NAME)

# Compilar cada .c a .o (si cambia el .h, también recompila)
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para compilar la librería externa usando su Makefile
$(LIB):
	$(MAKE) -C $(LIBDIR)

# Limpiar archivos objeto
clean:
	$(RM) $(OBJ)
	$(MAKE) -C $(LIBDIR) clean

# Limpiar objetos y ejecutable
fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBDIR) fclean

# Recompilación completa
re: fclean all

.PHONY: all clean fclean re