/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:14:32 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/03/24 14:05:14 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(void)
{
    char        *input;
    t_token     *tokens;
    t_token     *temp;
    extern char **environ;

    tokens = NULL;
    while (1)
    {
        input = readline("minishill$ ");
        if (!input)
            break;
        if (*input)
        {
            add_history(input);
            if (!ft_strncmp(input, "exit", 5))
            {
                free(input);
                exit(0);
            }
            tokens = tokenize(input);
            if (tokens)
            {
                if (ft_strncmp(tokens->value, "cd", 3) == 0)
                {
                    builtin_cd(tokens, environ);
                    free_tokens(tokens);
                    free(input);
                    continue;
                }
				if (ft_strncmp(tokens->value, "pwd", 4) == 0)
                {
                    builtin_pwd(tokens, environ);
                    free_tokens(tokens);
                    free(input);
                    continue;
                }
				if (ft_strncmp(tokens->value, "env", 4) == 0)
                {
                    builtin_env(tokens, environ);
                    free_tokens(tokens);
                    free(input);
                    continue;
                }
            }
        }
        if (tokens)
        {
            expand_tokens(tokens, environ);
            temp = tokens;
            while (temp)
            {
                printf("Token: %s (Type: %s)\n", temp->value, 
                       get_token_type_name(temp->type));
                temp = temp->next;
            }
            free_tokens(tokens);
        }
        free(input);
    }
    free_tokens(tokens);
    return (0);
}
