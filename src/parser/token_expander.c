/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:03:41 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/03/15 18:34:01 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_value(const char *name, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, ft_strlen(name)) == 0
			&& env[i][ft_strlen(name)] == '=')
			return (env[i] + ft_strlen(name) + 1);
		i++;
	}
	return (NULL);
}

int	append_expanded(char **buffer, size_t *pos, size_t *cap, char *value)
{
	size_t	len;

	if (!value)
		return (1);
	len = ft_strlen(value);
	if (!ensure_capacity(buffer, cap, *pos + len + 1))
		return (0);
	ft_strlcpy(*buffer + *pos, value, *cap - *pos + 1);
	*pos += len;
	return (1);
}

int	expand_var(const char *input, int *i, char **buffer, size_t *pos,
		size_t *cap, char **env)
{
	char	var_name[256];
	size_t	k;

	k = 0;
	(*i)++;
	while (input[*i] && (isalnum(input[*i]) || input[*i] == '_') && k < 255)
	{
		var_name[k++] = input[*i];
		(*i)++;
	}
	var_name[k] = '\0';
	if (k == 0)
		return (append_expanded(buffer, pos, cap, "$"));
	return (append_expanded(buffer, pos, cap, get_env_value(var_name, env)));
}

char	*expand_string(const char *input, char **env)
{
	char	*buffer;
	size_t	pos;
	size_t	cap;
	int		i;

	buffer = allocate_res_buff((char *)input);
	if (!buffer)
		return (NULL);
	pos = 0;
	cap = ft_strlen(input) * 4 + 1;
	if (cap < 1024)
		cap = 1024;
	i = 0;
	while (input[i])
	{
		if (input[i] == '~' && (i == 0 || is_whitespace(input[i - 1])))
			append_expanded(&buffer, &pos, &cap, get_env_value("HOME", env));
		else if (input[i] == '$')
			expand_var(input, &i, &buffer, &pos, &cap, env);
		else
			append_char(buffer, &pos, input[i]);
		i++;
	}
	return (buffer);
}

void	expand_tokens(t_token *tokens, char **env)
{
	t_token	*current;
	char	*expanded;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			expanded = expand_string(current->value, env);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
}
