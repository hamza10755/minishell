/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 13:33:06 by hbelaih           #+#    #+#             */
/*   Updated: 2025/03/13 19:43:46 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	char	*input;
	t_token	*tokens;
	t_token	*temp;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		tokens = tokenize(input);
		temp = tokens;
		while (temp)
		{
			printf("Token: %s (Type: %d)\n", temp->value, temp->type);
			temp = temp->next;
		}
		free_tokens(tokens);
		free(input);
	}
	return (0);
}
