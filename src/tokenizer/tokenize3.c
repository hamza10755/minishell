/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:53:24 by hbelaih           #+#    #+#             */
/*   Updated: 2025/03/13 14:49:36 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	process_quote_content(const char *input, int *i, char *buffer, size_t *j,
		char quote_char)
{
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
	{
		append_char(buffer, j, input[*i]);
		(*i)++;
	}
	if (input[*i] == quote_char)
	{
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

void	handle_word(const char *input, int *i, char *buffer, size_t *j,
		int *in_word)
{
	*in_word = 1;
	append_char(buffer, j, input[*i]);
	(*i)++;
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
