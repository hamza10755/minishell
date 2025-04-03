/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:41:01 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/04 02:24:06 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int  validate_identifier(const char *arg)
{
    size_t  i;

    if (!arg[0] || ft_isdigit(arg[0]) || arg[0] == '=')
    {
        ft_putstr_fd("minishell: export: `", STDERR_FILENO);
        ft_putstr_fd((char *)arg, STDERR_FILENO);
        ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
        return (0);  // Invalid identifier
    }
    if (arg[0] == '-' || (arg[0] == '+' && ft_isalpha(arg[1])))
    {
        ft_putstr_fd("minishell: export: invalid option\n", STDERR_FILENO);
        return (2);  // Option error
    }
    i = 0;
    while (arg[i] && arg[i] != '=')
    {
        if (!ft_isalnum(arg[i]) && arg[i] != '_' && (arg[i] != '+' || arg[i + 1] != '='))
        {
            ft_putstr_fd("minishell: export: `", STDERR_FILENO);
            ft_putstr_fd((char *)arg, STDERR_FILENO);
            ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
            return (0);  // Invalid identifier
        }
        i++;
    }
    return (1);  // Valid
}

static int  set_or_append_var(char *arg, char **env)
{
    char    *key;
    char    *value;
    char    *equal_sign;
    char    *new;

    equal_sign = ft_strchr(arg, '=');
    if (!equal_sign)
    {
        if (setenv(arg, "", 1) != 0)
            return (1);
        return (0);  // No value, set to empty
    }
    key = ft_substr(arg, 0, equal_sign - arg);
    if (!key)
        return (1);
    value = ft_strdup(equal_sign + 1);
    if (!value)
    {
        free(key);
        return (1);
    }
    if (key[ft_strlen(key) - 1] == '+')
    {
        key[ft_strlen(key) - 1] = '\0';
        char *old = get_env_value(key, env);
        if (old)
            new = ft_strjoin(old, value);
        else
            new = ft_strdup(value);
        if (!new || setenv(key, new, 1) != 0)
        {
            free(key); free(value); free(new);
            return (1);
        }
        free(new);
    }
    else if (setenv(key, value, 1) != 0)
    {
        free(key);
        free(value);
        return (1);
    }
    free(key);
    free(value);
    return (0);
}

static void print_export(char **env)
{
    int i;

    i = 0;
    while (env[i])
    {
        printf("declare -x %s\n", env[i]);
        i++;
    }
}

int builtin_export(t_token *tokens, char **env)
{
    t_token *current;
    int     status;

    if (!tokens->next || !tokens->next->value)
    {
        print_export(env);
        return (0);  // No args, print env
    }
    current = tokens->next;  // Skip "export"
    status = 0;
    while (current)
    {
        int valid = validate_identifier(current->value);
        if (valid == 2)
            return (2);  // Option error, stop
        if (valid == 0)
            status = 1;  // Invalid identifier, continue
        else if (set_or_append_var(current->value, env) != 0)
            status = 1;  // Set/append failed
        current = current->next;
    }
    return (status);
}