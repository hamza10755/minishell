/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:30:00 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/22 00:16:18 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	validate_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] == '+' && str[i + 1] == '=')
		return (1);
	if (str[i] == '=')
		return (1);
	return (0);
}

static int	set_or_append_var(t_env **env, char *var)
{
	char	*name;
	char	*value;
	char	*temp;
	t_env	*current;
	t_env	*new;
	int		is_append;
	char	*name_with_equals;

	is_append = 0;
	if (!validate_identifier(var))
		return (1);
	name = ft_strdup(var);
	if (!name)
		return (1);
	value = ft_strchr(name, '=');
	if (value)
	{
		if (value > name && *(value - 1) == '+')
		{
			is_append = 1;
			*(value - 1) = '\0';
		}
		*value = '\0';
		value++;
	}
	current = *env;
	while (current)
	{
		if (ft_strncmp(current->value, name, ft_strlen(name)) == 0
			&& current->value[ft_strlen(name)] == '=')
		{
			if (value)
			{
				if (is_append)
				{
					temp = ft_strjoin(current->value + ft_strlen(name) + 1,
							value);
					if (!temp)
					{
						free(name);
						return (1);
					}
					free(current->value);
					name_with_equals = ft_strjoin(name, "=");
					if (!name_with_equals)
					{
						free(name);
						free(temp);
						return (1);
					}
					current->value = ft_strjoin(name_with_equals, temp);
					free(name_with_equals);
					free(temp);
				}
				else
				{
					free(current->value);
					temp = ft_strjoin(name, "=");
					if (!temp)
					{
						free(name);
						return (1);
					}
					current->value = ft_strjoin(temp, value);
					free(temp);
				}
				free(name);
				return (current->value == NULL);
			}
			free(name);
			return (0);
		}
		current = current->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
	{
		free(name);
		return (1);
	}
	new->value = name;
	if (value)
	{
		temp = ft_strjoin(name, "=");
		if (!temp)
		{
			free(name);
			free(new);
			return (1);
		}
		new->value = ft_strjoin(temp, value);
		free(temp);
		free(name);
		if (!new->value)
		{
			free(new);
			return (1);
		}
	}
	new->next = NULL;
	if (!*env)
		*env = new;
	else
	{
		current = *env;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	return (0);
}

static void	print_export(t_env *env)
{
	char	**env_array;
	int		i;
	char	*value;

	env_array = convert_env_to_array(env);
	if (!env_array)
		return ;
	sort_env_vars(env_array);
	i = 0;
	while (env_array[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		value = ft_strchr(env_array[i], '=');
		if (value)
		{
			*value = '\0';
			ft_putstr_fd(env_array[i], STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(value + 1, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
			*value = '=';
		}
		else
			ft_putendl_fd(env_array[i], STDOUT_FILENO);
		i++;
	}
	free_env_array(env_array);
}

int	builtin_export(t_token *tokens, char **env_array)
{
	int		ret;
	t_env	*env;

	init_global_env(env_array);
	if (!tokens->next)
	{
		print_export(get_env());
		return (0);
	}
	ret = 0;
	tokens = tokens->next;
	while (tokens)
	{
		if (!validate_identifier(tokens->value))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(tokens->value, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ret = 1;
		}
		else
		{
			env = get_env();
			if (set_or_append_var(&env, tokens->value))
				ret = 1;
			set_env(env);
		}
		tokens = tokens->next;
	}
	return (ret);
}
