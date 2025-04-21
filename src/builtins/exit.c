/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:50:00 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/21 23:50:50 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_exit_code(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_token *tokens, int *exit_status)
{
	int code;

	if (!tokens->next)
	{
		*exit_status = 0;
		exit(0);
	}
	if (!is_valid_exit_code(tokens->next->value))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(tokens->next->value, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		*exit_status = 2;
		exit(2);
	}
	code = ft_atoi(tokens->next->value);
	if (tokens->next->next)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		*exit_status = 1;
		return (1);
	}
	*exit_status = code;
	exit(code);
}