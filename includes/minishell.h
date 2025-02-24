/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:33:33 by hbelaih           #+#    #+#             */
/*   Updated: 2025/02/24 15:08:22 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include "libft.h"

typedef struct s_command
{
    char **args;
    char *input_file;
    char *output_file;
    int append;
    struct s_command *next;
} t_command;

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_shell
{
    t_command *commands;
    t_env *env;
    int status;
} t_shell;
typedef struct s_token
{
    char *value;
    int type;
    struct s_token *next;
} t_token;

# endif