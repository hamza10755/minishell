/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:41:01 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/17 22:25:24 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	validate_identifier(const char *arg)
{
	int	i;

	if (!arg || !arg[0] || ft_isdigit(arg[0]))
		return (0);
	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	set_or_append_var(const char *arg)
{
	char	*equal_sign;
	char	*name;
	char	*value;
	int		overwrite;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
	{
		name = ft_strdup(arg);
		if (!name)
			return (1);
		value = ft_strdup("");
		overwrite = 0;
	}
	else
	{
		name = ft_substr(arg, 0, equal_sign - arg);
		if (!name)
			return (1);
		value = ft_strdup(equal_sign + 1);
		if (!value)
		{
			free(name);
			return (1);
		}
		overwrite = 1;
	}

	if (setenv(name, value, overwrite) != 0)
	{
		free(name);
		free(value);
		return (1);
	}

	free(name);
	free(value);
	return (0);
}

static void	print_export(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(env[i], STDOUT_FILENO);
		i++;
	}
}

int	builtin_export(t_token *tokens, char **env)
{
	int		ret;

	if (!tokens->next)
	{
		print_export(env);
		return (0);
	}

	ret = 0;
	while (tokens->next)
	{
		tokens = tokens->next;
		if (!validate_identifier(tokens->value))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(tokens->value, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ret = 1;
			continue;
		}
		if (set_or_append_var(tokens->value))
			ret = 1;
	}
	return (ret);
}
