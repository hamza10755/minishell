NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./includes
LDFLAGS = -lreadline -lncurses

SRC = src
LIBFTDIR = libft 

SRCS = parser/token_parser.c parser/token_main.c parser/token_expander.c parser/token_debug.c parser/token_builder.c parser/token_utils.c builtins/cd.c builtins/pwd.c builtins/env.c builtins/echo.c builtins/unset.c builtins/export.c main.c 

SRC_FILES = $(addprefix $(SRC)/, $(SRCS))
OBJ_FILES = $(SRC_FILES:.c=.o)

LIBFT = $(LIBFTDIR)/libft.a

all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(LIBFT) $(OBJ_FILES)
	@$(CC) $(OBJ_FILES) -L$(LIBFTDIR) -lft -o $(NAME) $(LDFLAGS)

$(LIBFT):
	@make -C $(LIBFTDIR)

clean:
	@rm -f $(OBJ_FILES)
	@make -C $(LIBFTDIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re
