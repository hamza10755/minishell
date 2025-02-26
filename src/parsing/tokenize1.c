/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:21:35 by hbelaih           #+#    #+#             */
/*   Updated: 2025/02/26 16:49:26 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	token_type(const char *str, int pos)
{
	char	c;
	char	next;

	c = str[pos];
	next = str[pos + 1];
	if (c == ';')
		return (TOKEN_SEMICOLON);
	if (c == '|')
		return (TOKEN_PIPE);
	if (c == '<' && next == '<')
		return (TOKEN_HEREDOC);
	if (c == '>' && next == '>')
		return (TOKEN_APPEND);
	if (c == '<' || c == '>')
		return (TOKEN_REDIR);
	if (c == ' ' || c == '\t' || c == '\n')
		return (TOKEN_WHITESPACE);
	if (c == '\'' || c == '\"')
		return (TOKEN_QUOTE);
	if (c == '\\')
		return (TOKEN_ESCAPE);
	return (TOKEN_WORD);
}

void	handle_quote(const char *input, int *i, char *buffer, size_t *j,
		t_token **tokens)
{
	char	quote_char;

	quote_char = input[*i];
	buffer[(*j)++] = quote_char;
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
		buffer[(*j)++] = input[(*i)++];
	if (input[*i] == quote_char)
		buffer[(*j)++] = input[(*i)++];
	flush_buffer(buffer, j, tokens, TOKEN_WORD);
}

void	handle_heredoc_append(const char *input, int *i, char *buffer,
		size_t *j, t_token **tokens, int current_type)
{
	if (input[*i + 2] == input[*i])
	{
		printf("Syntax error: too many redirection operators\n");
		*i += 3;
		return ;
	}
	flush_buffer(buffer, j, tokens, TOKEN_WORD);
	buffer[0] = input[*i];
	buffer[1] = input[*i + 1];
	buffer[2] = '\0';
	add_token(tokens, buffer, current_type);
	*i += 2;
}

void	handle_redir_pipe_semicolon(const char *input, int *i, char *buffer,
		size_t *j, t_token **tokens, int current_type)
{
	flush_buffer(buffer, j, tokens, TOKEN_WORD);
	buffer[0] = input[*i];
	buffer[1] = '\0';
	add_token(tokens, buffer, current_type);
	(*i)++;
}

void	handle_special_tokens(const char *input, int *i, char *buffer,
		size_t *j, t_token **tokens, int current_type)
{
	if (current_type == TOKEN_QUOTE)
		handle_quote(input, i, buffer, j, tokens);
	else if (current_type == TOKEN_HEREDOC || current_type == TOKEN_APPEND)
		handle_heredoc_append(input, i, buffer, j, tokens, current_type);
	else if (current_type == TOKEN_REDIR || current_type == TOKEN_PIPE
		|| current_type == TOKEN_SEMICOLON)
		handle_redir_pipe_semicolon(input, i, buffer, j, tokens, current_type);
}