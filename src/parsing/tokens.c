/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:21:35 by hbelaih           #+#    #+#             */
/*   Updated: 2025/02/26 14:39:21 by hbelaih          ###   ########.fr       */
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

t_token	*tokenize(const char *input)
{
	t_token	*tokens;
	char	buffer[4096];
	int		current_type;
	char	quote_char;
	int		i;
	size_t	j;

	tokens = NULL;
	i = 0;
	j = 0;
	while (input[i])
	{
		current_type = token_type(input, i);
		if (current_type == TOKEN_WHITESPACE)
		{
			if (j > 0)
			{
				buffer[j] = '\0';
				add_token(&tokens, buffer, TOKEN_WORD);
				j = 0;
			}
			i++;
			continue ;
		}
		if (current_type == TOKEN_QUOTE)
		{
			quote_char = input[i];
			buffer[j++] = quote_char;
			i++;
			while (input[i] && input[i] != quote_char)
				buffer[j++] = input[i++];
			if (input[i] == quote_char)
			{
				buffer[j++] = quote_char;
				i++;
			}
			buffer[j] = '\0';
			add_token(&tokens, buffer, TOKEN_WORD);
			j = 0;
			continue ;
		}
		if ((current_type == TOKEN_HEREDOC || current_type == TOKEN_APPEND)
			&& input[i + 1])
		{
			if (input[i + 2] == input[i])
			{
				ft_printf("Syntax error: too many redirection operators\n");
				return (NULL);
			}
			buffer[0] = input[i];
			buffer[1] = input[i + 1];
			buffer[2] = '\0';
			if (j > 0)
			{
				buffer[j] = '\0';
				add_token(&tokens, buffer, TOKEN_WORD);
				j = 0;
			}
			add_token(&tokens, buffer, current_type);
			i += 2;
			continue ;
		}
		if (current_type == TOKEN_REDIR)
		{
			if (input[i + 1] == input[i])
			{
				ft_printf("Syntax error: too many redirection operators\n");
				return (NULL);
			}
			if (j > 0)
			{
				buffer[j] = '\0';
				add_token(&tokens, buffer, TOKEN_WORD);
				j = 0;
			}
			buffer[0] = input[i];
			buffer[1] = '\0';
			add_token(&tokens, buffer, current_type);
			i++;
			continue ;
		}
		if (current_type == TOKEN_PIPE)
		{
			if (j > 0)
			{
				buffer[j] = '\0';
				add_token(&tokens, buffer, TOKEN_WORD);
				j = 0;
			}
			buffer[0] = input[i];
			buffer[1] = '\0';
			add_token(&tokens, buffer, TOKEN_PIPE);
			i++;
			continue ;
		}
		if (current_type == TOKEN_SEMICOLON)
		{
			if (j > 0)
			{
				buffer[j] = '\0';
				add_token(&tokens, buffer, TOKEN_WORD);
				j = 0;
			}
			buffer[0] = input[i];
			buffer[1] = '\0';
			add_token(&tokens, buffer, TOKEN_SEMICOLON);
			i++;
			continue ;
		}
		if (j < sizeof(buffer) - 1)
			buffer[j++] = input[i];
		i++;
	}
	if (j > 0)
	{
		buffer[j] = '\0';
		add_token(&tokens, buffer, TOKEN_WORD);
	}
	return (tokens);
}
