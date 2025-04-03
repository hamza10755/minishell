/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 18:32:59 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/03 19:27:15 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int  is_valid_identifier(char *arg)
{
    if (!arg || !arg[0] || arg[0] == '-')
        return (0);
    return (1);
}

static int  remove_variable(char *arg)
{
    if (!is_valid_identifier(arg))
    {
        ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
        ft_putstr_fd(arg, STDERR_FILENO);
        ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
        return (-1);
    }
    if (unsetenv(arg) != 0)
    {
        ft_putstr_fd("minishell: unset: failed to remove '", STDERR_FILENO);
        ft_putstr_fd(arg, STDERR_FILENO);
        ft_putstr_fd("'\n", STDERR_FILENO);
        return (-1);
    }
    return (0);
}

int builtin_unset(t_token *tokens, char **env)
{
    t_token *current;
    int     status;

    (void)env;
    if (!tokens || !tokens->next || !tokens->next->value)
    {
        ft_putstr_fd("minishell: unset: missing argument\n", STDERR_FILENO);
        return (-1);
    }
    current = tokens->next;
    status = 0;
    while (current)
    {
        if (remove_variable(current->value) == -1)
            status = -1;
        current = current->next;
    }
    return (status);
}