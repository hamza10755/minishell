/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:37:40 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/04/17 22:36:47 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <dirent.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>

// Token structure
typedef struct s_token
{
	char			*value;
	int				type;
	int				exit_status;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

// Token types
# define TOKEN_WORD 0
# define TOKEN_WHITESPACE 1
# define TOKEN_PIPE 2
# define TOKEN_REDIR 3
# define TOKEN_HEREDOC 4
# define TOKEN_APPEND 5
# define TOKEN_SEMICOLON 6
# define TOKEN_QUOTE 7
# define TOKEN_ESCAPE 8

// token_expander.c
char				*get_env_value(const char *name, char **env);
int					append_expanded(char **buffer, size_t *pos, size_t *cap,
						char *value);
int					expand_var(const char *input, int *i, char **buffer,
						size_t *pos, size_t *cap, char **env, int *exit_status);
char				*expand_string(const char *input, char **env, int *exit_status);
void				expand_tokens(t_token *tokens, char **env, int *exit_status);

// token_utils.c
int					is_quote(char c);
int					is_whitespace(char c);
void				append_char(char *buffer, size_t *index, char c);
t_token				*create_new_token(const char *value, int type);
char				*allocate_res_buff(char *str);

// token_builder.c
int					is_operator(char c);
t_token				*add_token(t_token **tokens, const char *value, int type);
void				flush_buffer(char *buffer, size_t *index, t_token **tokens);
int					handle_double_operator(const char *input, int *i, char *buf,
						size_t *j, int type);
int					handle_single_operator(const char *input, int *i, char *buf,
						size_t *j, int type);

// token_parser.c
int					handle_operator(const char *input, int *i, char *buffer,
						size_t *j, t_token **tokens);
int					process_quote_content(const char *input, int *i,
						char *buffer, size_t *j, char quote_char);
int					handle_quotes(const char *input, int *i, char *buffer,
						size_t *j);
void				handle_whitespace(int *i, char *buffer, size_t *j,
						t_token **tokens, int *in_word);
t_token				*handle_operator_in_word(char *buffer, size_t *j,
						t_token **tokens, int *in_word);

// token_main.c
void				handle_word(const char *input, int *i, char *buffer,
						size_t *j, int *in_word);
int					process_current_char(const char *input, int *i,
						char *buffer, t_token **tokens, size_t *j,
						int *in_word);
t_token				*process_input(const char *input, char *buffer);
t_token				*init_process_vars(size_t *j, int *i, int *in_word);
t_token				*tokenize(const char *input);

// token_debug.c
const char			*get_token_type_name(int type);
void				print_tokens(t_token *tokens);
void				free_tokens(t_token *tokens);
int					ensure_capacity(char **res, size_t *cap, size_t needed);
int					copy_expanded_value(char **res, size_t *j, char *expanded,
						size_t *capacity);

// builtins
int					builtin_cd(t_token *tokens, char **env);
int					builtin_pwd(t_token *tokens, char **env);
int					builtin_env(t_token *tokens, char **env);
int					builtin_echo(t_token *tokens, int *exit_status);
int					builtin_unset(t_token *tokens, char **env);
int					builtin_export(t_token *tokens, char **env);

int					execute_command(t_token *tokens, char **env, int *exit_status);
int					execute_simple_command(t_token *tokens, char **env, int *exit_status);
int					handle_redirections(t_token *tokens, int *fd_in, int *fd_out);
int					setup_redirection(t_token *token, int *fd_in, int *fd_out);
void				restore_redirections(int fd_in, int fd_out);

int					is_builtin(t_token *token);
int					execute_builtin(t_token *tokens, char **env, int *exit_status);

//execution
int     execute_command(t_token *tokens, char **env, int *exit_status);
int     execute_simple_command(t_token *tokens, char **env, int *exit_status);
int     execute_pipeline(t_token *tokens, char **env);

// Redirection handling
int     handle_redirections(t_token *tokens, int *fd_in, int *fd_out);
int     setup_redirection(t_token *token, int *fd_in, int *fd_out);
void    restore_redirections(int fd_in, int fd_out);

// Path resolution
char    *resolve_command_path(char *command, char **env);
char    *search_in_path(char *command, char *path);

// Environment handling
char    **convert_env_to_array(char **env);
void    free_env_array(char **env_array);
char    **convert_tokens_to_args(t_token *tokens);

// Process handling
void    handle_child_process(t_token *tokens, char **env, int fd_in, int fd_out);
void    handle_parent_process(pid_t pid, int *status);


// Builtin handling
int     is_builtin(t_token *token);
int     execute_builtin(t_token *tokens, char **env, int *exit_status);
#endif