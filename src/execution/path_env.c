/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 05:00:00 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/22 00:19:32 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	*resolve_command_path(char *command, char **env)
{
	char	*path;
	char	*full_path;

	if (ft_strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	path = get_env_value("PATH", env);
	if (!path)
		return (NULL);
	full_path = search_in_path(command, path);
	return (full_path);
}

char	*search_in_path(char *command, char *path)
{
	char	**dirs;
	char	*full_path;
	char	*temp;
	int		i;

	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		temp = ft_strjoin(dirs[i], "/");
		if (!temp)
		{
			free_array(dirs);
			return (NULL);
		}
		full_path = ft_strjoin(temp, command);
		free(temp);
		if (!full_path)
		{
			free_array(dirs);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			free_array(dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(dirs);
	return (NULL);
}

void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

char	**convert_tokens_to_args(t_token *tokens)
{
	char **args;
	int count;
	int i;
	t_token *current;

	count = 0;
	current = tokens;
	while (current && current->type == TOKEN_WORD)
	{
		count++;
		current = current->next;
	}

	args = malloc((count + 1) * sizeof(char *));
	if (!args)
		return (NULL);

	i = 0;
	current = tokens;
	while (i < count)
	{
		args[i] = ft_strdup(current->value);
		if (!args[i])
		{
			free_array(args);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	args[count] = NULL;
	return (args);
}
