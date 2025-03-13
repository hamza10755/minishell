/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:37:40 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/03/13 20:12:16 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "libft.h"
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}	t_token;

enum e_token_type
{
	TOKEN_WORD,
	TOKEN_WHITESPACE,
	TOKEN_PIPE,
	TOKEN_REDIR,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_SEMICOLON,
	TOKEN_QUOTE,
	TOKEN_ESCAPE
};

t_token	*tokenize(const char *input);
t_token *process_input(const char *input, char *buffer);
t_token	*create_new_token(const char *value, int type);
t_token	*add_token(t_token **tokens, const char *value, int type);
void	free_tokens(t_token *tokens);
void	print_tokens(t_token *tokens);
void	print_token_type(int type);
int		is_quote(char c);
int		is_whitespace(char c);
int		is_operator(char c);
void	append_char(char *buffer, size_t *j, char c);
void	flush_buffer(char *buffer, size_t *j, t_token **tokens);
int		handle_operator(const char *input, int *i, char *buffer,
			size_t *j, t_token **tokens);
int		handle_quotes(const char *input, int *i, char *buffer, size_t *j);
void	handle_word(const char *input, int *i, char *buffer,
			size_t *j, int *in_word);
void	handle_whitespace(int *i, char *buffer, size_t *j,
			t_token **tokens, int *in_word);
char	*ft_strdup(const char *s);
int		handle_double_operator(const char *input, int *i, char *buffer,
			size_t *j, int type);
int		handle_single_operator(const char *input, int *i, char *buffer,
			size_t *j, int type);
int		process_quote_content(const char *input, int *i, char *buffer,
			size_t *j, char quote_char);
t_token	*handle_operator_in_word(char *buffer, size_t *j,
			t_token **tokens, int *in_word);
int		process_current_char(const char *input, int *i, char *buffer,
			t_token **tokens, size_t *j, int *in_word);
t_token	*init_process_vars(size_t *j, int *i, int *in_word);

#endif