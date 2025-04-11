/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:03:41 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/11 04:32:29 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int  append_expanded(char **buffer, size_t *pos, size_t *cap, char *value)
{
    size_t  len;

    if (!value)
        return (1);
    len = ft_strlen(value);
    if (!ensure_capacity(buffer, cap, *pos + len + 1))
        return (0);
    ft_strlcpy(*buffer + *pos, value, *cap - *pos + 1);
    *pos += len;
    return (1);
}

static int  is_valid_var_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}

static int  count_var_chars(const char *str)
{
    int i;

    i = 0;
    while (str[i] && is_valid_var_char(str[i]))
        i++;
    return (i);
}

static char *extract_var_name(const char *str)
{
    int     len;
    char    *var;

    len = count_var_chars(str);
    var = malloc(len + 1);
    if (!var)
        return (NULL);
    ft_strlcpy(var, str, len + 1);
    return (var);
}

static int  check_single_quotes(const char *input, int pos)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (i < pos)
    {
        if (input[i] == '\'')
            count++;
        i++;
    }
    return (count % 2);
}

static int  handle_var_expansion(const char *input, int *i, char **buffer,
    size_t *pos, size_t *cap, char **env)
{
    char    var_name[256];
    int     k;
    char    *value;

    k = 0;
    (*i)++;
    if (check_single_quotes(input, *i))
    {
        append_char(*buffer, pos, '$');
        return (1);
    }
    while (input[*i] && is_valid_var_char(input[*i]) && k < 255)
        var_name[k++] = input[(*i)++];
    var_name[k] = '\0';
    if (k == 0)
    {
        append_char(*buffer, pos, '$');
        return (1);
    }
    value = get_env_value(var_name, env);
    return (append_expanded(buffer, pos, cap, value));
}

static int  handle_tilde_expansion(const char *input, int i, char **buffer,
    size_t *pos, size_t *cap, char **env)
{
    if (i == 0 || is_whitespace(input[i - 1]))
        return (append_expanded(buffer, pos, cap, get_env_value("HOME", env)));
    append_char(*buffer, pos, '~');
    return (1);
}

static int  process_char(const char *input, int *i, char **buffer,
    size_t *pos, size_t *cap, char **env)
{
    if (input[*i] == '$')
        return (handle_var_expansion(input, i, buffer, pos, cap, env));
    if (input[*i] == '~')
        return (handle_tilde_expansion(input, *i, buffer, pos, cap, env));
    append_char(*buffer, pos, input[(*i)++]);
    return (1);
}

char    *expand_string(const char *input, char **env)
{
    char    *buffer;
    size_t  pos;
    size_t  cap;
    int     i;

    if (!input || !ft_strchr(input, '$'))
        return (ft_strdup(input ? input : ""));
    buffer = allocate_res_buff((char *)input);
    if (!buffer)
        return (NULL);
    pos = 0;
    cap = ft_strlen(input) * 4 + 1;
    i = 0;
    while (input[i])
    {
        if (!process_char(input, &i, &buffer, &pos, &cap, env))
        {
            free(buffer);
            return (NULL);
        }
    }
    buffer[pos] = '\0';
    return (buffer);
}

static void process_token_value(t_token *token, char **env)
{
    char    *temp;
    int     i;
    int     in_quotes;
    char    quote_type;

    i = 0;
    in_quotes = 0;
    quote_type = 0;
    temp = ft_strdup("");
    while (token->value[i])
    {
        if ((token->value[i] == '\'' || token->value[i] == '\"') && !in_quotes)
        {
            in_quotes = 1;
            quote_type = token->value[i];
        }
        else if (in_quotes && token->value[i] == quote_type)
        {
            in_quotes = 0;
            quote_type = 0;
        }
        else if (token->value[i] == '$' && (!in_quotes || quote_type == '\"'))
        {
            char    *var;
            char    *value;
            char    *new_temp;

            var = extract_var_name(token->value + i + 1);
            value = get_env_value(var, env);
            if (value)
            {
                new_temp = ft_strjoin(temp, value);
                free(temp);
                temp = new_temp;
            }
            i += ft_strlen(var) + 1;
            free(var);
            continue;
        }
        else
        {
            char    str[2];
            char    *new_temp;

            str[0] = token->value[i];
            str[1] = '\0';
            new_temp = ft_strjoin(temp, str);
            free(temp);
            temp = new_temp;
        }
        i++;
    }
    free(token->value);
    token->value = temp;
}

void    expand_tokens(t_token *tokens, char **env)
{
    t_token *current;

    current = tokens;
    while (current)
    {
        if (current->type == TOKEN_WORD)
            process_token_value(current, env);
        current = current->next;
    }
}

char    *get_env_value(const char *name, char **env)
{
    int i;

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
