NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./includes
LDFLAGS = -lreadline -lncurses

SRC = src
LIBFTDIR = libft 

SRCS = tokenizer/tokenize1.c tokenizer/tokenize2.c tokenizer/tokenize3.c tokenizer/tokenize4.c tokenizer/tokenize5.c main.c

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
