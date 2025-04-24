/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:30:00 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/24 18:28:20 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int builtin_cd(t_token *tokens, char **env_array)
{
    char *dir_path = NULL;
    char current_dir[PATH_MAX];
    char new_dir[PATH_MAX];
    int ret = 0;

    init_global_env(env_array);
    
    if (getcwd(current_dir, PATH_MAX) == NULL)
    {
        perror("minishell: cd");
        return (1);
    }

    if (!tokens->next)
    {
        dir_path = get_env_value("HOME", env_array);
        if (!dir_path)
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
            return (1);
        }
    }
    else if (ft_strncmp(tokens->next->value, "-", 2) == 0)
    {
        dir_path = get_env_value("OLDPWD", env_array);
        if (!dir_path)
        {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
            return (1);
        }
        ft_putendl_fd(dir_path, STDOUT_FILENO); 
    }
    else
    {
        dir_path = tokens->next->value;
    }

    if (chdir(dir_path) == -1)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(dir_path, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        perror("");
        return (1);
    }

    if (getcwd(new_dir, PATH_MAX) == NULL)
    {
        perror("minishell: cd");
        return (1);
    }

    t_token export_token;
    t_token oldpwd_token;
    char *oldpwd_value = ft_strjoin("OLDPWD=", current_dir);
    
    if (!oldpwd_value)
        return (1);
        
    export_token.value = "export";
    export_token.next = &oldpwd_token;
    oldpwd_token.value = oldpwd_value;
    oldpwd_token.next = NULL;
    
    ret = builtin_export(&export_token, env_array);
    free(oldpwd_value);
    
    char *pwd_value = ft_strjoin("PWD=", new_dir);
    if (!pwd_value)
        return (1);
        
    oldpwd_token.value = pwd_value;
    
    ret |= builtin_export(&export_token, env_array);
    free(pwd_value);
    
    return (ret);
}