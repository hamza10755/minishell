/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:57:45 by hbelaih           #+#    #+#             */
/*   Updated: 2025/04/17 22:19:41 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>

static int	compare_env_vars(const char *a, const char *b)
{
	int	i;
	int	len_a;
	int	len_b;

	// Find the length up to '=' for both strings
	len_a = 0;
	while (a[len_a] && a[len_a] != '=')
		len_a++;
	len_b = 0;
	while (b[len_b] && b[len_b] != '=')
		len_b++;

	// Compare the variable names
	i = 0;
	while (i < len_a && i < len_b && a[i] == b[i])
		i++;
	return (a[i] - b[i]);
}

static void	sort_env_vars(char **env, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (compare_env_vars(env[j], env[j + 1]) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

int	builtin_env(t_token *tokens, char **env)
{
	char	**sorted_env;
	int		count;
	int		i;

	(void)tokens;
	if (!env)
		return (1);
	count = 0;
	while (env[count])
		count++;
	sorted_env = malloc(sizeof(char *) * (count + 1));
	if (!sorted_env)
		return (1);
	i = 0;
	while (i < count)
	{
		sorted_env[i] = env[i];
		i++;
	}
	sorted_env[count] = NULL;
	sort_env_vars(sorted_env, count);
	i = 0;
	while (sorted_env[i])
	{
		ft_putendl_fd(sorted_env[i], STDOUT_FILENO);
		i++;
	}
	free(sorted_env);
	return (0);
}
