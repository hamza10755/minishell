/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:30:00 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/21 23:29:02 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	validate_identifier(const char *arg)
{
	int	i;

	if (!arg || !arg[0] || ft_isdigit(arg[0]))
		return (0);
	i = 0;
	while (arg[i])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	remove_env_var(t_env **env, const char *arg)
{
	t_env	*current;
	t_env	*prev;
	size_t	arg_len;

	arg_len = ft_strlen(arg);
	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->value, arg, arg_len) == 0
			&& (current->value[arg_len] == '='
				|| current->value[arg_len] == '\0'))
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

int	builtin_unset(t_token *tokens, char **env_array)
{
	static t_env	*env = NULL;
	int				ret;

	if (!env)
	{
		env = init_env(env_array);
		if (!env)
			return (1);
	}
	if (!tokens->next)
		return (0);
	ret = 0;
	tokens = tokens->next;
	while (tokens)
	{
		if (!validate_identifier(tokens->value))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(tokens->value, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ret = 1;
		}
		else
			remove_env_var(&env, tokens->value);
		tokens = tokens->next;
	}
	return (ret);
}
