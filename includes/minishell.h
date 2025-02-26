/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:33:33 by hbelaih           #+#    #+#             */
/*   Updated: 2025/02/27 00:17:19 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <unistd.h>

typedef struct s_command
{
	char				**args;
	char				*input_file;
	char				*output_file;
	int					append;
	struct s_command	*next;
}						t_command;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_shell
{
	t_command			*commands;
	t_env				*env;
	int					status;
}						t_shell;
typedef struct s_token
{
	char				*value;
	int					type;
	struct s_token		*next;
}						t_token;

typedef enum e_token_type
{
	TOKEN_WORD = 0,
	TOKEN_SEMICOLON = 1,
	TOKEN_PIPE = 2,
	TOKEN_REDIR = 3,
	TOKEN_WHITESPACE = 4,
	TOKEN_QUOTE = 5,
	TOKEN_ESCAPE = 6,
	TOKEN_HEREDOC = 7,
	TOKEN_APPEND = 8
}						t_token_type;

t_token					*tokenize(const char *input);
void	flush_buffer(char *buffer, size_t *j, t_token **tokens, int type);
t_token	*add_token(t_token **tokens, const char *value, int type);
void	handle_special_tokens(const char *input, int *i, char *buffer,
		size_t *j, t_token **tokens, int current_type);
int	token_type(const char *str, int pos);
void					free_tokens(t_token *tokens);

#endif