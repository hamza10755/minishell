/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 08:08:38 by hbelaih           #+#    #+#             */
/*   Updated: 2025/03/03 14:14:08 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


char *expand_varibles(char *str, char **env)
{
    (void)env;
    if (ft_strncmp(str, "$USER",100000) == 0)
        return ft_strdup("hamza");
    return ft_strdup(str);
}

char *expand_tilde(char *str, char **env)
{
    (void)env;
    if (ft_strncmp(str, "~",1000) == 0)
        return ft_strdup("/home/hamza");
    return ft_strdup(str);
}

void expand_tokens(t_token *tokens, char **env);

// Utility function to create a new token
t_token *new_token(int type, char *value)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->type = type;
    token->value = ft_strdup(value);
    token->next = NULL;
    return token;
}

/*expand all of the tokens in the list and handling the tilde (~)*/
void    expand_tokens(t_token *tokens, char **env)
{
    t_token *curr;
    char    *expanded;

    curr = tokens;
    while (curr)
    {
        if (curr->type == TOKEN_WORD)
            {
                expanded = expand_varibles(curr->value, env);
                if (expanded)
                {
                    free(curr->value);
                    curr->value = expanded;
                }
            }
        expanded = expand_tilde(curr->value, env);
        if (expanded)
        {
            free(curr->value);
            curr->value = expanded;
        }
        curr = curr->next;
    }
}
/*multipied by 4 to allocate the right size
  because the case is over the size of malloc(strlen + 1)
  for exanple $HAMZA it my be /user/hamza so its 10+ characters
  instead of 5*/
char *allocate_res_buff(char *str)
{
    char *res;
    size_t init_size;
    
    if (!str)
        return (NULL);
    init_size = ft_strlen(str) * 4 + 1;
    if (init_size < 1024)
        init_size = 1024;
    res = (char *)malloc(sizeof(char) * (init_size));
    if (!res)
        return (NULL);
    return (res);
}
/*incuring there's enough capacity and returning 1 if theres enough 
  if not it makes a larger size copy's everything to the new buffer
  free's the old one and returning 1*/
int ensure_capacity(char **res, size_t *cap, size_t needed)
{
    char *new_res;
    size_t new_cap;

    if (*cap >= needed)
        return (1);
    new_cap = *cap * 2;
    if (new_cap < needed)
        new_cap = needed;
    new_res = (char *)malloc(sizeof(char) * (new_cap));
    if (!new_res)
        return (0);
    ft_strlcpy(new_res, *res, *cap + 1);
    free(*res);
    *res = new_res;
    *cap = new_cap;
    return (1);
}
/*this function copy's the value to it's */
int    copy_expanded_value(char **res, size_t *j, char *expanded, size_t *capacity)
{
    size_t vl;
    
    if (!expanded)
        return (1);
    vl = ft_strlen(expanded);
    if (!ensure_capacity(res, j, *j + vl + 1))
        return (0);
    ft_strlcpy(*res + *j, expanded, *capacity + 1);
    *j += vl;
    return (1);
}
int main()
{
    char *env[] = {"USER=hamza", "HOME=/home/hamza", NULL};
    
    t_token *head = new_token(TOKEN_WORD, "$USER");
    head->next = new_token(TOKEN_WORD, "Hello");
    head->next->next = new_token(TOKEN_WORD, "~");
    
    printf("Before Expansion:\n");
    for (t_token *curr = head; curr; curr = curr->next)
        printf("%s\n", curr->value);
    
    expand_tokens(head, env);
    
    printf("\nAfter Expansion:\n");
    for (t_token *curr = head; curr; curr = curr->next)
        printf("%s\n", curr->value);
    free_tokens(head);
    
    return 0;
}