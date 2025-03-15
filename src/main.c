/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:14:32 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/03/15 21:47:25 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	char 		*input;
	t_token 	*tokens;
	t_token 	*temp;
	extern char **environ;

	tokens = NULL;
	while (1)
	{
		input = readline("minishill$ ");
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			if (!ft_strncmp(input, "exit", 5))
			{
				free(input);
				exit(0);
			}
		}
		tokens = tokenize(input);
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