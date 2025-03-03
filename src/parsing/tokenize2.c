/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:34:51 by hbelaih           #+#    #+#             */
/*   Updated: 2025/03/03 11:06:30 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_word_chars(const char *input, int *i, char *buffer, size_t *j)
{
	buffer[(*j)++] = input[*i];
	(*i)++;
}
/*adds a new token used almost everywhere*/
void	flush_buffer(char *buffer, size_t *j, t_token **tokens, int type)
{
	if (*j > 0)
	{
		buffer[*j] = '\0';
		add_token(tokens, buffer, type);
		*j = 0;
	}
}
/*checks if its an empty linked list it puts it at first if not starts to put it in last*/
t_token	*add_token(t_token **tokens, const char *value, int type)
{
	t_token	*new_token;
	t_token	*temp;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	if (!*tokens)
		*tokens = new_token;
	else
	{
		temp = *tokens;
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
	}
	return (new_token);
}
/*walk throught the tokens and deciding what is the token type*/
static void	process_token(const char *input, int *i, char *buffer, size_t *j,
		t_token **tokens)
{
	int	current_type;
	int	prev_i;

	current_type = token_type(input, *i);
	if (current_type == TOKEN_WHITESPACE)
	{
		flush_buffer(buffer, j, tokens, TOKEN_WORD);
		(*i)++;
	}
	else if (current_type == TOKEN_QUOTE || current_type == TOKEN_HEREDOC
		|| current_type == TOKEN_APPEND || current_type == TOKEN_REDIR
		|| current_type == TOKEN_PIPE || current_type == TOKEN_SEMICOLON)
	{
		flush_buffer(buffer, j, tokens, TOKEN_WORD);
		prev_i = *i;
		handle_special_tokens(input, i, buffer, j, tokens, current_type);
		if (prev_i == *i && (current_type == TOKEN_HEREDOC
				|| current_type == TOKEN_APPEND))
			*i = -1;
	}
	else
		handle_word_chars(input, i, buffer, j);
}

t_token	*tokenize(const char *input)
{
	t_token	*tokens;
	char	buffer[2048];
	int		i;
	size_t	j;

	tokens = NULL;
	i = 0;
	j = 0;
	while (input[i])
	{
		process_token(input, &i, buffer, &j, &tokens);
		if (i == -1)
			break ;
		if (input[i] == '\0')
			break ;
	}
	flush_buffer(buffer, &j, &tokens, TOKEN_WORD);
	return (tokens);
}
