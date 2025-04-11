/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:41:01 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/11 04:26:38 by hamzabillah      ###   ########.fr       */
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
        return (0);
    }
    if (arg[0] == '-' || (arg[0] == '+' && ft_isalpha(arg[1])))
    {
        ft_putstr_fd("minishell: export: invalid option\n", STDERR_FILENO);
        return (2);
    }
    i = 0;
    while (arg[i] && arg[i] != '=')
    {
        if (!ft_isalnum(arg[i]) && arg[i] != '_' && (arg[i] != '+' || arg[i + 1] != '='))
        {
            ft_putstr_fd("minishell: export: `", STDERR_FILENO);
            ft_putstr_fd((char *)arg, STDERR_FILENO);
            ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
            return (0);
        }
        i++;
    }
    return (1);
}

static int  count_env_size(char **env)
{
    int i;

    i = 0;
    while (env[i])
        i++;
    return (i);
}

static int  copy_env_to_new(char **new_env, char **env, int size)
{
    int i;

    i = 0;
    while (i < size)
    {
        new_env[i] = env[i];
        i++;
    }
    return (0);
}

static int  add_var_to_env(char *arg, char **env)
{
    int     size;
    char    **new_env;

    size = count_env_size(env);
    new_env = malloc((size + 2) * sizeof(char *));
    if (!new_env)
        return (1);
    if (copy_env_to_new(new_env, env, size) != 0)
    {
        free(new_env);
        return (1);
    }
    new_env[size] = ft_strdup(arg);
    if (!new_env[size])
    {
        free(new_env);
        return (1);
    }
    new_env[size + 1] = NULL;
    extern char **environ;
    environ = new_env;
    return (0);
}

static int  handle_append_var(char *key, char *value, char **env)
{
    char    *old;
    char    *new;

    key[ft_strlen(key) - 1] = '\0';
    old = get_env_value(key, env);
    if (old)
        new = ft_strjoin(old, value);
    else
        new = ft_strdup(value);
    if (!new || setenv(key, new, 1) != 0)
    {
        free(new);
        return (1);
    }
    free(new);
    return (0);
}

static int  set_or_append_var(char *arg, char **env)
{
    char    *key;
    char    *value;
    char    *equal_sign;

    equal_sign = ft_strchr(arg, '=');
    if (!equal_sign)
    {
        if (!get_env_value(arg, env))
            return (add_var_to_env(arg, env));
        return (0);
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
        if (handle_append_var(key, value, env) != 0)
        {
            free(key);
            free(value);
            return (1);
        }
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

static void print_var_with_value(char *env_var)
{
    char    *equal_sign;
    char    *value;

    equal_sign = ft_strchr(env_var, '=');
    if (equal_sign)
    {
        *equal_sign = '\0';
        value = equal_sign + 1;
        printf("export %s=\"%s\"\n", env_var, value);
        *equal_sign = '=';
    }
    else
        printf("export %s\n", env_var);
}

static void print_export(char **env)
{
    int i;

    i = 0;
    while (env[i])
    {
        print_var_with_value(env[i]);
        i++;
    }
}

static int  process_export_arg(t_token *current, char **env)
{
    int valid;

    valid = validate_identifier(current->value);
    if (valid == 2)
        return (2);
    if (valid == 0)
        return (1);
    if (set_or_append_var(current->value, env) != 0)
        return (1);
    return (0);
}

int builtin_export(t_token *tokens, char **env)
{
    t_token *current;
    int     status;
    int     result;

    if (!tokens->next || !tokens->next->value)
    {
        print_export(env);
        return (0);
    }
    current = tokens->next;
    status = 0;
    while (current)
    {
        result = process_export_arg(current, env);
        if (result == 2)
            return (2);
        if (result == 1)
            status = 1;
        current = current->next;
    }
    return (status);
}
