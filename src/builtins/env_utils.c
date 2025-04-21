/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:33:00 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/21 23:35:37 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*init_env(char **env)
{
	t_env	*head;
	t_env	*current;
	t_env	*new;
	int		i;

	head = NULL;
	i = 0;
	while (env[i])
	{
		new = malloc(sizeof(t_env));
		if (!new)
		{
			free_env(head);
			return (NULL);
		}
		new->value = ft_strdup(env[i]);
		if (!new->value)
		{
			free(new);
			free_env(head);
			return (NULL);
		}
		new->next = NULL;
		if (!head)
		{
			head = new;
			current = head;
		}
		else
		{
			current->next = new;
			current = current->next;
		}
		i++;
	}
	return (head);
}

void	free_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}

char	**convert_env_to_array(t_env *env)
{
	char **array;
	int count;
	int i;
	t_env *current;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	i = 0;
	current = env;
	while (current)
	{
		array[i] = ft_strdup(current->value);
		if (!array[i])
		{
			while (i--)
				free(array[i]);
			free(array);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	array[i] = NULL;
	return (array);
}