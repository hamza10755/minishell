/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:26:48 by hbelaih           #+#    #+#             */
/*   Updated: 2025/04/17 22:39:25 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_target_dir(t_token *tokens, char **env, char **old_pwd)
{
	char	*dir_path;

	dir_path = NULL;
	*old_pwd = getcwd(NULL, 0);
	if (!*old_pwd)
		printf("minishell: cd: cannot get current directory\n");
	if (!*old_pwd)
		return (NULL);
	if (tokens && tokens->next && tokens->next->value)
	{
		dir_path = tokens->next->value;
		if (ft_strncmp(dir_path, "-", 2) == 0 || ft_strncmp(dir_path, "+",2) == 0)
		{
			dir_path = get_env_value("OLDPWD", env);
			if (!dir_path)
				printf("minishell: cd: OLDPWD not set\n");
		}
	}
	else
	{
		dir_path = get_env_value("HOME", env);
		if (!dir_path)
			printf("minishell: cd: HOME not set\n");
	}
	return (dir_path);
}

static int	change_and_update_dir(char *dir_path, char *old_pwd,
		t_token *tokens)
{
	char	*new_pwd;

	if (!dir_path || chdir(dir_path) == -1)
	{
		if (dir_path)
			printf("minishell: cd: %s: No such file or directory\n", dir_path);
		free(old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		printf("minishell: cd: cannot get current directory\n");
		free(old_pwd);
		return (1);
	}
	setenv("OLDPWD", old_pwd, 1);
	setenv("PWD", new_pwd, 1);
	if (tokens && tokens->next && tokens->next->value
		&& ft_strncmp(tokens->next->value, "-", 2) == 0)
		printf("%s\n", new_pwd);
	free(old_pwd);
	free(new_pwd);
	return (0);
}

int	builtin_cd(t_token *tokens, char **env)
{
	char *dir_path;
	char *old_pwd;

	dir_path = get_target_dir(tokens, env, &old_pwd);
	if (!old_pwd)
		return (1);
	if (!dir_path)
	{
		free(old_pwd);
		return (1);
	}
	return (change_and_update_dir(dir_path, old_pwd, tokens));
}