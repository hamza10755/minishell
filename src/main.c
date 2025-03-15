/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:14:32 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/03/15 18:26:50 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(void)
{
    char    *input;
    t_token *tokens;
    t_token *temp;
    extern char **environ;

    while (1)
    {
        input = readline("minishill$ ");
        if (!input)
            break ;
        if (*input)
            add_history(input);
        tokens = tokenize(input);
        if (tokens)
        {
            expand_tokens(tokens, environ);
            temp = tokens;
            while (temp)
            {
                printf("Token: %s (Type: ", temp->value);
                print_token_type(temp->type);
                printf(")\n");
                temp = temp->next;
            }
        }
        free_tokens(tokens);
        free(input);
    }
    return (0);
}