/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 18:20:14 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/17 21:48:17 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_echo(t_token *tokens, int *exit_status)
{
	int		newline;
	t_token	*current;

	(void)exit_status;
	newline = 1;
	current = tokens->next;
	if (current && ft_strncmp(current->value, "-n", 3) == 0)
	{
		newline = 0;
		current = current->next;
	}
	while (current)
	{
		ft_putstr_fd(current->value, STDOUT_FILENO);
		if (current->next)
			ft_putchar_fd(' ', STDOUT_FILENO);
		current = current->next;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
