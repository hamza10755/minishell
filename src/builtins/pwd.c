/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:48:46 by hbelaih           #+#    #+#             */
/*   Updated: 2025/03/24 13:49:51 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_pwd(t_token *tokens, char **env)
{
	char *pwd;
	(void)tokens;

	pwd = get_env_value("PWD", env);
	if (pwd)
	{
		printf("%s\n", pwd);
		return (0);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		printf("minishell: pwd: cannot get current directory\n");
		return (1);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}