/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:57:45 by hbelaih           #+#    #+#             */
/*   Updated: 2025/03/24 13:58:31 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include   "../../includes/minishell.h"

int builtin_env(t_token *tokens, char **env)
{
    int i;

    (void)tokens;
    i = 0;
    while (env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }

    return (0);
}