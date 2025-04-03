/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 18:20:14 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/03 18:29:05 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int  has_n_flag(t_token *tokens)
{
    if (tokens && tokens->next && tokens->next->value
        && ft_strncmp(tokens->next->value, "-n", 3) == 0)
        return (1);
    return (0);
}

int builtin_echo(t_token *tokens, char **env)
{
    t_token *current;
    int     suppress_newline;

    (void)env;
    suppress_newline = has_n_flag(tokens);
    current = tokens->next;
    if (suppress_newline)
        current = current->next;

    while (current)
    {
        printf("%s", current->value);
        if (current->next)
            printf(" ");
        current = current->next;
    }
    if (!suppress_newline)
        printf("\n");
    return (0);
}
