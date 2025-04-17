/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 05:00:00 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/17 22:43:49 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int execute_command(t_token *tokens, char **env, int *exit_status)
{
    t_token *current;
    int     status;
    int     pipe_fd[2];
    int     prev_pipe_read = STDIN_FILENO;

    if (!tokens)
        return (0);

    current = tokens;
    while (current)
    {
        if (current->type == TOKEN_PIPE)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
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
            while (current && current->type != TOKEN_PIPE && current->type != TOKEN_SEMICOLON)
                current = current->next;
            if (current)
                current = current->next;
        }
    }
    if (prev_pipe_read != STDIN_FILENO)
        close(prev_pipe_read);
    return (0);
}

int execute_simple_command(t_token *tokens, char **env, int *exit_status)
{
    int     fd_in;
    int     fd_out;
    pid_t   pid;
    int     status;

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
        perror("fork");
        *exit_status = 1;
        return (1);
    }
    else if (pid == 0)
    {
        handle_child_process(tokens, env, fd_in, fd_out);
        exit(1);
    }
    else
    {
        handle_parent_process(pid, &status);
        restore_redirections(fd_in, fd_out);
        *exit_status = WEXITSTATUS(status);
    }
    return (*exit_status);
}

int execute_pipeline(t_token *tokens, char **env)
{
    int     pipe_fd[2];
    pid_t   pid;
    int     status;
    t_token *current;
    int     prev_pipe_read;

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
        continue;

    if (prev_pipe_read != STDIN_FILENO)
        close(prev_pipe_read);

    return (WEXITSTATUS(status));
}

void handle_child_process(t_token *tokens, char **env, int fd_in, int fd_out)
{
    char    *cmd_path;
    char    **args;
    char    **env_array;

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

    cmd_path = resolve_command_path(tokens->value, env);
    if (!cmd_path)
    {
        fprintf(stderr, "minishell: %s: command not found\n", tokens->value);
        exit(127);
    }

    args = convert_tokens_to_args(tokens);
    env_array = convert_env_to_array(env);

    execve(cmd_path, args, env_array);
    perror("execve");
    free(cmd_path);
    free_env_array(env_array);
    exit(126);
}

void handle_parent_process(pid_t pid, int *status)
{
    waitpid(pid, status, 0);
    if (WIFEXITED(*status))
        *status = WEXITSTATUS(*status);
    else if (WIFSIGNALED(*status))
        *status = 128 + WTERMSIG(*status);
} 