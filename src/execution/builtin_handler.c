/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 02:48:32 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/17 22:37:17 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int is_builtin(t_token *token)
{
    if (!token || !token->value)
        return (0);
    if (ft_strncmp(token->value, "cd", 3) == 0)
        return (1);
    if (ft_strncmp(token->value, "pwd", 4) == 0)
        return (1);
    if (ft_strncmp(token->value, "env", 4) == 0)
        return (1);
    if (ft_strncmp(token->value, "echo", 5) == 0)
        return (1);
    if (ft_strncmp(token->value, "unset", 6) == 0)
        return (1);
    if (ft_strncmp(token->value, "export", 7) == 0)
        return (1);
    return (0);
}

int execute_builtin(t_token *tokens, char **env, int *exit_status)
{
    int status;

    if (ft_strncmp(tokens->value, "cd", 3) == 0)
        status = builtin_cd(tokens, env);
    else if (ft_strncmp(tokens->value, "pwd", 4) == 0)
        status = builtin_pwd(tokens, env);
    else if (ft_strncmp(tokens->value, "env", 4) == 0)
        status = builtin_env(tokens, env);
    else if (ft_strncmp(tokens->value, "echo", 5) == 0)
        status = builtin_echo(tokens, exit_status);
    else if (ft_strncmp(tokens->value, "unset", 6) == 0)
        status = builtin_unset(tokens, env);
    else if (ft_strncmp(tokens->value, "export", 7) == 0)
        status = builtin_export(tokens, env);
    else
        status = 0;

    *exit_status = status;
    return (status);
} 