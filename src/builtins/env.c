/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:30:00 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/22 00:10:20 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	compare_env_vars(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	sort_env_vars(char **env)
{
	int		i;
	int		j;
	char	*temp;
	int		swapped;

	i = 0;
	while (env[i])
	{
		swapped = 0;
		j = 0;
		while (env[j + 1])
		{
			if (compare_env_vars(env[j], env[j + 1]) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
				swapped = 1;
			}
			j++;
		}
		if (!swapped)
			break ;
		i++;
	}
}

int	builtin_env(t_token *tokens, char **env_array)
{
	char	**sorted_env;
	int		i;
	char	*value;

	init_global_env(env_array);
	if (tokens->next)
	{
		ft_putstr_fd("minishell: env: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	sorted_env = convert_env_to_array(get_env());
	if (!sorted_env)
		return (1);
	sort_env_vars(sorted_env);
	i = 0;
	while (sorted_env[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		value = ft_strchr(sorted_env[i], '=');
		if (value)
		{
			*value = '\0';
			ft_putstr_fd(sorted_env[i], STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(value + 1, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
			*value = '=';
		}
		else
			ft_putendl_fd(sorted_env[i], STDOUT_FILENO);
		i++;
	}
	free_env_array(sorted_env);
	return (0);
}
