/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:30:00 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/21 23:23:55 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	update_env_var(t_env **env, const char *name, const char *value)
{
	t_env	*current;
	t_env	*new;
	char	*temp;
	char	*new_var;

	temp = ft_strjoin(name, "=");
	if (!temp)
		return (1);
	new_var = ft_strjoin(temp, value);
	free(temp);
	if (!new_var)
		return (1);
	current = *env;
	while (current)
	{
		if (ft_strncmp(current->value, name, ft_strlen(name)) == 0
			&& current->value[ft_strlen(name)] == '=')
		{
			free(current->value);
			current->value = new_var;
			return (0);
		}
		current = current->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
	{
		free(new_var);
		return (1);
	}
	new->value = new_var;
	new->next = *env;
	*env = new;
	return (0);
}

static char	*get_env_value_from_list(const char *name, t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->value, name, ft_strlen(name)) == 0
			&& current->value[ft_strlen(name)] == '=')
			return (current->value + ft_strlen(name) + 1);
		current = current->next;
	}
	return (NULL);
}

static char	*get_target_dir(t_token *tokens, t_env *env, char **old_pwd)
{
	char	*home;

	if (!tokens->next)
	{
		home = get_env_value_from_list("HOME", env);
		if (!home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (NULL);
		}
		return (ft_strdup(home));
	}
	if (ft_strncmp(tokens->next->value, "-", 2) == 0)
	{
		*old_pwd = get_env_value_from_list("OLDPWD", env);
		if (!*old_pwd)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
			return (NULL);
		}
		return (ft_strdup(*old_pwd));
	}
	return (ft_strdup(tokens->next->value));
}

int	builtin_cd(t_token *tokens, char **env_array)
{
	static t_env *env = NULL;
	char *dir_path;
	char *old_pwd;
	char *new_pwd;
	char cwd[PATH_MAX];

	if (!env)
	{
		env = init_env(env_array);
		if (!env)
			return (1);
	}
	old_pwd = getcwd(cwd, PATH_MAX);
	if (!old_pwd)
	{
		perror("minishell: cd");
		return (1);
	}
	dir_path = get_target_dir(tokens, env, &old_pwd);
	if (!dir_path)
		return (1);
	if (chdir(dir_path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(dir_path, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		free(dir_path);
		return (1);
	}
	new_pwd = getcwd(cwd, PATH_MAX);
	if (!new_pwd)
	{
		perror("minishell: cd");
		free(dir_path);
		return (1);
	}
	if (update_env_var(&env, "OLDPWD", old_pwd) != 0 || update_env_var(&env,
			"PWD", new_pwd) != 0)
	{
		free(dir_path);
		return (1);
	}
	free(dir_path);
	return (0);
}