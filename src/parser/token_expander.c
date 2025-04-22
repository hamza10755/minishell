/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:03:41 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/22 11:35:44 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static int	count_var_chars(const char *str)
{
	int	count;

	count = 0;
	while (str[count] && is_valid_var_char(str[count]))
		count++;
	return (count);
}

static char	*extract_var_name(const char *str, int len)
{
	char	*name;

	name = malloc(len + 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, str, len + 1);
	return (name);
}

static int handle_special_var(const char *name, char **buffer, size_t *pos, size_t *cap, int *exit_status)
{
	char	*value;

	if (ft_strncmp(name, "?", 2) == 0)
	{
		value = ft_itoa(*exit_status);
		if (!value)
			return (1);
		if (append_expanded(buffer, pos, cap, value) != 0)
		{
			free(value);
			return (1);
		}
		free(value);
		return (0);
	}
	return (-1);
}

int expand_var(const char *input, int *i, char **buffer, size_t *pos, size_t *cap, char **env, int *exit_status)
{
	char	*name;
	char	*value;
	int		len;
	int		special;
	int		in_quotes;

	in_quotes = 0;
	if (*i > 0 && input[*i - 1] == '\"')
		in_quotes = 1;

	(*i)++;
	if (input[*i] == '?')
	{
		(*i)++;
		return (handle_special_var("?", buffer, pos, cap, exit_status));
	}
	if (ft_isdigit(input[*i]))
	{
		(*i)++;
		while (input[*i] && is_valid_var_char(input[*i]))
		{
			append_char(*buffer, pos, input[*i]);
			(*i)++;
		}
		return (0);
	}
	len = count_var_chars(input + *i);
	if (len == 0)
	{
		if (append_expanded(buffer, pos, cap, "$") != 0)
			return (1);
		return (0);
	}
	name = extract_var_name(input + *i, len);
	if (!name)
		return (1);
	*i += len;
	special = handle_special_var(name, buffer, pos, cap, exit_status);
	if (special != -1)
	{
		free(name);
		return (special);
	}
	value = get_env_value(name, env);
	free(name);
	if (!value)
		return (0);

	if (in_quotes)
	{
		if (append_expanded(buffer, pos, cap, value) != 0)
		{
			free(value);
			return (1);
		}
	}
	else
	{
		char	*new_value;
		int		j;
		int		k;
		int		space_count;

		new_value = malloc(ft_strlen(value) + 1);
		if (!new_value)
		{
			free(value);
			return (1);
		}
		j = 0;
		k = 0;
		space_count = 0;
		while (value[j])
		{
			if (value[j] == ' ')
			{
				if (space_count == 0)
					new_value[k++] = ' ';
				space_count++;
			}
			else
			{
				new_value[k++] = value[j];
				space_count = 0;
			}
			j++;
		}
		new_value[k] = '\0';
		if (append_expanded(buffer, pos, cap, new_value) != 0)
		{
			free(value);
			free(new_value);
			return (1);
		}
		free(new_value);
	}
	free(value);
	return (0);
}

char *expand_string(const char *input, char **env, int *exit_status)
{
    char    *result;
    size_t  pos;
    size_t  cap;
    int     i;
    char    *tmp;
    if (!input)
        return (NULL);
    cap = 16;
    result = malloc(cap);
    if (!result)
        return (NULL);
    result[0] = '\0';
    pos = 0;
    i = 0;
    while (input[i])
    {
        if (pos + 2 >= cap)
        {
            cap *= 2;
            tmp = malloc(cap);
            if (!tmp)
            {
                free(result);
                return (NULL);
            }
            ft_memcpy(tmp, result, pos);
            tmp[pos] = '\0';
            free(result);
            result = tmp;
        }
        if (input[i] == '\'')
        {
            i++;
            while (input[i] && input[i] != '\'')
            {
                if (pos + 2 >= cap)
                {
                    cap *= 2;
                    tmp = malloc(cap);
                    if (!tmp)
                    {
                        free(result);
                        return (NULL);
                    }
                    ft_memcpy(tmp, result, pos);
                    tmp[pos] = '\0';
                    free(result);
                    result = tmp;
                }
                append_char(result, &pos, input[i]);
                i++;
            }
            if (input[i] == '\'')
                i++;
        }
        else if (input[i] == '\"')
        {
            i++;
            while (input[i] && input[i] != '\"')
            {
                if (input[i] == '$')
                {
                    if (expand_var(input, &i, &result, &pos, &cap, env, exit_status) == -1)
                    {
                        free(result);
                        return (NULL);
                    }
                }
                else
                {
                    if (pos + 2 >= cap)
                    {
                        cap *= 2;
                        tmp = malloc(cap);
                        if (!tmp)
                        {
                            free(result);
                            return (NULL);
                        }
                        ft_memcpy(tmp, result, pos);
                        tmp[pos] = '\0';
                        free(result);
                        result = tmp;
                    }
                    append_char(result, &pos, input[i]);
                    i++;
                }
            }
            if (input[i] == '\"')
                i++;
        }
        else if (input[i] == '$')
        {
            if (expand_var(input, &i, &result, &pos, &cap, env, exit_status) == -1)
            {
                free(result);
                return (NULL);
            }
        }
        else
        {
            append_char(result, &pos, input[i]);
            i++;
        }
    }
    return (result);
}

void expand_tokens(t_token *tokens, char **env, int *exit_status)
{
    t_token *current;
    t_token *next;
    char    *expanded;
    char    **split_tokens;
    int     i;

    current = tokens;
    while (current)
    {
        next = current->next;
        if (current->type == TOKEN_WORD)
        {
            expanded = expand_string(current->value, env, exit_status);
            if (expanded)
            {
                if (current == tokens && ft_strchr(expanded, ' ') != NULL)
                {
                    split_tokens = ft_split(expanded, ' ');
                    if (split_tokens)
                    {
                        free(current->value);
                        current->value = ft_strdup(split_tokens[0]);
                        
                        i = 1;
                        while (split_tokens[i])
                        {
                            t_token *new_token = malloc(sizeof(t_token));
                            if (!new_token)
                                break;
                            new_token->value = ft_strdup(split_tokens[i]);
                            new_token->type = TOKEN_WORD;
                            new_token->exit_status = *exit_status;
                            new_token->next = NULL;
                            new_token->prev = NULL;
                            
                            // Insert into list
                            new_token->next = current->next;
                            if (current->next)
                                current->next->prev = new_token;
                            new_token->prev = current;
                            current->next = new_token;
                            
                            current = new_token;
                            
                            i++;
                        }                        
                        i = 0;
                        while (split_tokens[i])
                            free(split_tokens[i++]);
                        free(split_tokens);
                    }
                    free(expanded);
                }
                else
                {
                    free(current->value);
                    current->value = expanded;
                }
            }
        }
        current = next;
    }
}

char *get_env_value(const char *name, char **env)
{
    t_env *current = get_env();
    while (current)
    {
        if (ft_strncmp(current->value, name, ft_strlen(name)) == 0
            && current->value[ft_strlen(name)] == '=')
        {
            return ft_strdup(current->value + ft_strlen(name) + 1);
        }
        current = current->next;
    }
    
    int i = 0;
    while (env && env[i])
    {
        if (ft_strncmp(env[i], name, ft_strlen(name)) == 0
            && env[i][ft_strlen(name)] == '=')
        {
            return ft_strdup(env[i] + ft_strlen(name) + 1);
        }
        i++;
    }
    
    return NULL;
}

