/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:51:46 by hbelaih           #+#    #+#             */
/*   Updated: 2025/03/13 14:51:53 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*add_token(t_token **tokens, const char *value, int type)
{
	t_token	*new_token;
	t_token	*temp;

	if (!value || value[0] == '\0')
		return (NULL);
	new_token = create_new_token(value, type);
	if (!new_token)
		return (NULL);
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

void	flush_buffer(char *buffer, size_t *j, t_token **tokens)
{
	int	type;

	if (*j > 0)
	{
		buffer[*j] = '\0';
		type = TOKEN_WORD;
		if (ft_strncmp(buffer, "|", 3) == 0)
			type = TOKEN_PIPE;
		else if (ft_strncmp(buffer, ";", 3) == 0)
			type = TOKEN_SEMICOLON;
		else if (ft_strncmp(buffer, "<", 3) == 0 || ft_strncmp(buffer, ">",
				3) == 0)
			type = TOKEN_REDIR;
		else if (ft_strncmp(buffer, "<<", 4) == 0)
			type = TOKEN_HEREDOC;
		else if (ft_strncmp(buffer, ">>", 4) == 0)
			type = TOKEN_APPEND;
		add_token(tokens, buffer, type);
		*j = 0;
	}
}

int	handle_double_operator(const char *input, int *i, char *buffer, size_t *j,
		int type)
{
	append_char(buffer, j, input[*i]);
	append_char(buffer, j, input[*i + 1]);
	*i += 2;
	return (type);
}

int	handle_single_operator(const char *input, int *i, char *buffer, size_t *j,
		int type)
{
	append_char(buffer, j, input[*i]);
	(*i)++;
	return (type);
}

int	handle_operator(const char *input, int *i, char *buffer, size_t *j,
		t_token **tokens)
{
	char	current;
	char	next;
	int		type;

	current = input[*i];
	next = input[*i + 1];
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
