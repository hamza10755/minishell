/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:54:32 by hbelaih           #+#    #+#             */
/*   Updated: 2025/03/13 14:53:42 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	process_current_char(const char *input, int *i, char *buffer,
		t_token **tokens, size_t *j, int *in_word)
{
	if (is_whitespace(input[*i]))
		handle_whitespace(i, buffer, j, tokens, in_word);
	else if (is_quote(input[*i]))
	{
		if (!handle_quotes(input, i, buffer, j))
			return (0);
		*in_word = 1;
	}
	else if (is_operator(input[*i]))
	{
		handle_operator_in_word(buffer, j, tokens, in_word);
		handle_operator(input, i, buffer, j, tokens);
	}
	else
		handle_word(input, i, buffer, j, in_word);
	return (1);
}

t_token	*init_process_vars(size_t *j, int *i, int *in_word)
{
	*j = 0;
	*i = 0;
	*in_word = 0;
	return (NULL);
}

t_token	*process_input(const char *input, char *buffer)
{
	t_token	*tokens;
	size_t	j;
	int		i;
	int		in_word;

	tokens = init_process_vars(&j, &i, &in_word);
	while (input[i])
	{
		if (!process_current_char(input, &i, buffer, &tokens, &j, &in_word))
		{
			printf("Error: unclosed quote\n");
			return (NULL);
		}
	}
	if (in_word || j > 0)
		flush_buffer(buffer, &j, &tokens);
	return (tokens);
}

t_token	*tokenize(const char *input)
{
	char	buffer[4096];

	return (process_input(input, buffer));
}

void	print_token_type(int type)
{
	const char	*type_names[] = {"WORD", "WHITESPACE", "PIPE", "REDIR",
		"HEREDOC", "APPEND", "SEMICOLON", "QUOTE", "ESCAPE"};

	printf("%s", type_names[type]);
}
