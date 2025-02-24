/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:21:35 by hbelaih           #+#    #+#             */
/*   Updated: 2025/02/24 17:08:41 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int token_type(char c)
{
    if (c == ';')
        return 1;
    if (c == '|')
        return 2;
    if (c == '<' || c == '>')
        return 3;
    if (c == ' ' || c == '\t' || c == '\n')
        return 4;
    if (c == '\'' || c == '\"')
        return 5;
    if (c == '\\')
        return 6;
    return 0;
}

t_token *create_token(const char *value, int type)
{
    t_token *new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token)
        return NULL;
    new_token->value = ft_strdup(value);
    if (!new_token->value)
    {
        free(new_token);
        return NULL;
    }
    new_token->type = type;
    new_token->next = NULL;
    return new_token;
}

void add_token(t_token **tokens, t_token *new_token)
{
    if (!*tokens)
    {
        *tokens = new_token;
    }
    else
    {
        t_token *temp = *tokens;
        while (temp->next)
            temp = temp->next;
        temp->next = new_token;
    }
}

t_token *tokenize(const char *input)
{
    t_token *tokens;
    char buffer[256];
    int i ;
    int j;

    i = 0;
    j = 0;
    tokens = NULL;
    while (input[i])
    {
        if (token_type(input[i]) > 0)
        {
            if (j > 0)
            {
                buffer[j] = '\0';
                add_token(&tokens, create_token(buffer, 0));
                j = 0;
            }
            buffer[0] = input[i];
            buffer[1] = '\0';
            add_token(&tokens, create_token(buffer, token_type(input[i])));
        }
        else
        {
            if (j < 255)
                buffer[j++] = input[i];
        }
        i++;
    }
    if (j > 0)
    {
        buffer[j] = '\0';
        add_token(&tokens, create_token(buffer, 0));
    }
    return tokens;
}

void free_tokens(t_token *tokens)
{
    t_token *temp;
    while (tokens)
    {
        temp = tokens;
        tokens = tokens->next;
        free(temp->value);
        free(temp);
    }
}
//testing
void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        ft_printf("Token value: %s, Token type: %d\n", tokens->value, tokens->type);
        tokens = tokens->next;
    }
}

void test_tokenize(char *input)
{
    t_token *tokens = tokenize(input);
    print_tokens(tokens);
    free_tokens(tokens);
}

int main()
{
    ft_printf("Test 1: Simple command\n");
    test_tokenize("echo hello world");

    ft_printf("\nTest 2: Command with semicolon\n");
    test_tokenize("echo hello; echo world");

    ft_printf("\nTest 3: Command with pipe\n");
    test_tokenize("echo hello | grep world");

    ft_printf("\nTest 4: Command with redirection\n");
    test_tokenize("cat < input.txt > output.txt");

    ft_printf("\nTest 5: Command with quotes\n");
    test_tokenize(">>|<<<<<>||||||||||>>|||||||||||||");

    ft_printf("\nTest 6: Command with backslash\n");
    test_tokenize("echo hello\\ world");

    return 0;
}