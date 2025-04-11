/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:06:25 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/11 04:34:50 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_operator(const char *input, int *i, char *buffer, size_t *j,
		t_token **tokens)
{
	char	current;
	char	next;
	char	next_next;
	int		type;

	current = input[*i];
	next = input[*i + 1];
	if (input[*i + 2])
		next_next = input[*i + 2];
	else
		next_next = '\0';
	if ((current == '>' && next == '>' && next_next == '>') || (current == '<'
			&& next == '<' && next_next == '<'))
	{
		flush_buffer(buffer, j, tokens);
		printf("minishell: syntax error near unexpected token\n");
		*i += 3;
		return (-1);
	}
	flush_buffer(buffer, j, tokens);
	if (current == '<' && next == '<')
		type = handle_double_operator(input, i, buffer, j, TOKEN_HEREDOC);
	else if (current == '>' && next == '>')
		type = handle_double_operator(input, i, buffer, j, TOKEN_APPEND);
	else if (current == '|')
		type = handle_single_operator(input, i, buffer, j, TOKEN_PIPE);
	else if (current == ';')
		type = handle_single_operator(input, i, buffer, j, TOKEN_SEMICOLON);
	else
		type = handle_single_operator(input, i, buffer, j, TOKEN_REDIR);
	flush_buffer(buffer, j, tokens);
	return (type);
}

int	process_quote_content(const char *input, int *i, char *buffer, size_t *j,
		char quote_char)
{
	(*i)++;
	append_char(buffer, j, quote_char);
	while (input[*i] && input[*i] != quote_char)
	{
		append_char(buffer, j, input[*i]);
		(*i)++;
	}
	if (input[*i] == quote_char)
	{
		append_char(buffer, j, quote_char);
		(*i)++;
		return (1);
	}
	return (0);
}

int	handle_quotes(const char *input, int *i, char *buffer, size_t *j)
{
	char	quote_char;
	int		start_pos;

	quote_char = input[*i];
	start_pos = *i;
	if (!process_quote_content(input, i, buffer, j, quote_char))
	{
		*i = start_pos;
		return (0);
	}
	return (1);
}

void	handle_whitespace(int *i, char *buffer, size_t *j, t_token **tokens,
		int *in_word)
{
	if (*in_word)
	{
		flush_buffer(buffer, j, tokens);
		*in_word = 0;
	}
	(*i)++;
}

t_token	*handle_operator_in_word(char *buffer, size_t *j, t_token **tokens,
		int *in_word)
{
	if (*in_word)
	{
		flush_buffer(buffer, j, tokens);
		*in_word = 0;
	}
	return (*tokens);
}
