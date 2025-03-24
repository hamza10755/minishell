/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:26:48 by hbelaih           #+#    #+#             */
/*   Updated: 2025/03/24 13:54:00 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/minishell.h"

int builtin_cd(t_token *tokens, char **env)
{
    char *dir_path = NULL;
    char *new_pwd;

    if (tokens && tokens->next && tokens->next->value)
        dir_path = tokens->next->value;
    else
        dir_path = get_env_value("HOME", env);
    if (!dir_path)
    {
        printf("minishell: cd: HOME not set\n");
        return (1);
    }
    if (chdir(dir_path) == -1)
    {
        printf("minishell: cd: %s: No such file or directory\n", dir_path);
        return (1);
    }
    new_pwd = getcwd(NULL, 0);
    if (!new_pwd)
    {
        printf("minishell: cd: cannot get current directory\n");
        return (1);
    }
    setenv("PWD", new_pwd, 1);
    free(new_pwd);

    return (0);
}