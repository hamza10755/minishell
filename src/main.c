/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:14:32 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/17 21:44:21 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int g_exit_status = 0;

static int  is_command(t_token *token)
{
    if (!token || !token->value)
        return (0);
    if (token->type != TOKEN_WORD)
        return (0);
    if (is_builtin(token))
        return (0);
    if (token->prev && (token->prev->type == TOKEN_PIPE || 
        token->prev->type == TOKEN_REDIR || 
        token->prev->type == TOKEN_APPEND || 
        token->prev->type == TOKEN_HEREDOC))
        return (0);
    return (1);
}

static int  is_argument(t_token *token)
{
    if (!token || !token->value)
        return (0);
    if (token->type != TOKEN_WORD)
        return (0);
    if (is_builtin(token))
        return (0);
    if (is_command(token))
        return (0);
    return (1);
}

static void print_token_info(t_token *token)
{
    printf("Token: %s\n", token->value);
    printf("Type: %s\n", get_token_type_name(token->type));
    
    if (token->type == TOKEN_WORD)
    {
        if (is_builtin(token))
            printf("Category: Builtin Command\n");
        else if (is_command(token))
            printf("Category: Command\n");
        else if (is_argument(token))
            printf("Category: Argument\n");
    }
    else if (token->type == TOKEN_REDIR)
        printf("Category: Redirection\n");
    else if (token->type == TOKEN_PIPE)
        printf("Category: Pipe\n");
    else if (token->type == TOKEN_HEREDOC)
        printf("Category: Heredoc\n");
    else if (token->type == TOKEN_APPEND)
        printf("Category: Append Redirection\n");
    printf("-------------------\n");
}

int main(void)
{
    char        *input;
    t_token     *tokens;
    t_token     *temp;
    extern char **environ;
    int         exit_status = 0;

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
                exit(exit_status);
            }
            tokens = tokenize(input);
            if (tokens)
            {
                expand_tokens(tokens, environ, &exit_status);
                temp = tokens;
                while (temp)
                {
                    print_token_info(temp);
                    temp = temp->next;
                }
                execute_command(tokens, environ, &exit_status);
                free_tokens(tokens);
            }
        }
        free(input);
    }
    free_tokens(tokens);
    return (exit_status);
}
