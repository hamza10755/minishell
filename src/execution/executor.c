/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:30:00 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/22 00:00:53 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_env	*init_shell_env(char **env)
{
	static t_env	*shell_env = NULL;

	if (!shell_env)
		shell_env = init_env(env);
	return (shell_env);
}

static char	**get_env_array(t_env *env)
{
	return (convert_env_to_array(env));
}

static void	handle_child_process(t_token *tokens, t_env *env, int fd_in,
		int fd_out)
{
	char	**args;
	char	**env_array;
	char	*cmd_path;

	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	args = convert_tokens_to_args(tokens);
	if (!args)
		exit(1);
	env_array = get_env_array(env);
	if (!env_array)
	{
		free_env_array(args);
		exit(1);
	}
	cmd_path = resolve_command_path(args[0], env_array);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free_env_array(args);
		free_env_array(env_array);
		exit(127);
	}
	if (execve(cmd_path, args, env_array) == -1)
	{
		perror("minishell");
		free(cmd_path);
		free_env_array(args);
		free_env_array(env_array);
		exit(126);
	}
}

static void	handle_parent_process(pid_t pid, int *status)
{
	waitpid(pid, status, 0);
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		*status = WTERMSIG(*status) + 128;
}

int	execute_simple_command(t_token *tokens, char **env, int *exit_status)
{
	pid_t	pid;
	int		fd_in;
	int		fd_out;
	t_env	*shell_env;
	int		status;

	shell_env = init_shell_env(env);
	if (!shell_env)
		return (1);
	if (!tokens || !tokens->value || !tokens->value[0])
		return (0);
	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	if (handle_redirections(tokens, &fd_in, &fd_out) != 0)
	{
		*exit_status = 1;
		return (1);
	}
	if (is_builtin(tokens))
		return (execute_builtin(tokens, env, exit_status));
	pid = fork();
	if (pid == -1)
	{
		perror("minishell");
		*exit_status = 1;
		return (1);
	}
	else if (pid == 0)
	{
		handle_child_process(tokens, shell_env, fd_in, fd_out);
		exit(1);
	}
	else
	{
		handle_parent_process(pid, &status);
		restore_redirections(fd_in, fd_out);
		*exit_status = status;
	}
	return (0);
}

int	execute_command(t_token *tokens, char **env, int *exit_status)
{
	t_token	*current;
	int		status;
	int		pipe_fd[2];
	int		prev_pipe_read;

	if (!tokens)
		return (0);
	prev_pipe_read = STDIN_FILENO;
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("minishell");
				*exit_status = 1;
				return (1);
			}
			status = execute_simple_command(current, env, exit_status);
			if (status != 0)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
				return (status);
			}
			if (prev_pipe_read != STDIN_FILENO)
				close(prev_pipe_read);
			prev_pipe_read = pipe_fd[0];
			close(pipe_fd[1]);
			current = current->next;
		}
		else
		{
			status = execute_simple_command(current, env, exit_status);
			if (status != 0)
				return (status);
			while (current && current->type != TOKEN_PIPE
				&& current->type != TOKEN_SEMICOLON)
				current = current->next;
			if (current)
				current = current->next;
		}
	}
	if (prev_pipe_read != STDIN_FILENO)
		close(prev_pipe_read);
	return (0);
}

int	execute_pipeline(t_token *tokens, char **env)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;
	t_token	*current;
	int		prev_pipe_read;

	prev_pipe_read = STDIN_FILENO;
	current = tokens;
	while (current)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (1);
		}
		else if (pid == 0)
		{
			close(pipe_fd[0]);
			dup2(prev_pipe_read, STDIN_FILENO);
			if (current->next && current->next->type == TOKEN_PIPE)
				dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
			execute_simple_command(current, env, NULL);
			exit(0);
		}
		else
		{
			close(pipe_fd[1]);
			if (prev_pipe_read != STDIN_FILENO)
				close(prev_pipe_read);
			prev_pipe_read = pipe_fd[0];
			current = current->next;
			if (current && current->type == TOKEN_PIPE)
				current = current->next;
		}
	}
	while (waitpid(-1, &status, 0) > 0)
		continue ;
	if (prev_pipe_read != STDIN_FILENO)
		close(prev_pipe_read);
	return (WEXITSTATUS(status));
}
