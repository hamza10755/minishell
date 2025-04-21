/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_state.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:43:00 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/21 23:43:51 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_env	*g_env = NULL;

t_env	*get_env(void)
{
	return (g_env);
}

void	set_env(t_env *env)
{
	g_env = env;
}

void	init_global_env(char **env_array)
{
	if (!g_env)
		g_env = init_env(env_array);
}