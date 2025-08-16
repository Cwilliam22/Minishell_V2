/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfavre <alfavre@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 13:43:15 by alfavre           #+#    #+#             */
/*   Updated: 2025/08/16 13:43:15 by alfavre          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/*                                 INCLUDES                                   */
/* ************************************************************************** */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <signal.h>
# include <dirent.h>
# include <errno.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

/* ************************************************************************** */
/*                                 DEFINES                                    */
/* ************************************************************************** */

# define PROMPT "minishell$> "
# define MAX_PATH 4096
# define MAX_ARGS 1024

# define NO_QUOTED 0
# define HALF_QUOTED 1
# define QUOTED 2

/* ************************************************************************** */
/*                                    ENUM                                    */
/* ************************************************************************** */

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND,
	T_HEREDOC,
	T_EOF,
	T_VAR,
	T_ASS
}	t_token_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef enum e_exit_code
{
	SUCCESS = 0,
	GENERAL_ERROR = 1,
	SYNTAX_ERROR = 2,
	MISUSE_BUILTIN = 2,
	COMMAND_NOT_EXECUTABLE = 126,
	COMMAND_NOT_FOUND = 127,
	EXIT_SIGINT = 130,
	EXIT_SIGQUIT = 131
}	t_exit_code;

typedef enum e_fd
{
	FD_STDIN = 0,
	FD_STDOUT = 1,
	FD_STDERR = 2
}	t_fd;

typedef enum e_state_path
{
	PATH_SIMPLE = 0,
	PATH_RELATIVE = 1,
	PATH_ABSOLUTE = 2
}	t_state_path;

typedef enum e_signal_state
{
	SIGNAL_INTERACTIVE = 1,
	SIGNAL_EXECUTING = 2,
	SIGNAL_HEREDOC = 3
}	t_signal_state;

/* ************************************************************************** */
/*                                STRUCTURES                                  */
/* ************************************************************************** */

/* Token structure for lexical analysis */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_heredoc
{
	char	*delimiter;
	char	*content;
	int		id;
	int		quoted_delimiter;
	int		fd;
}	t_heredoc;

/* Redirection structure */
typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	t_heredoc		*heredoc;
	int				fd_in;
	int				fd_out;
	int				nb_hd;
	struct s_redir	*next;
}	t_redir;

/* Assignment structure for export commands */
typedef struct s_ass
{
	char			*key;
	char			*value;
	int				is_append;
	struct s_ass	*next;
}	t_ass;

/* Command structure */
typedef struct s_cmd
{
	char			**args;
	char			**args_expanded;
	t_ass			*assignments;
	t_redir			*redirections;
	char			*cmd_path;
	t_state_path	state_path;
	struct s_cmd	*next;
}	t_cmd;

/* Environment variable structure */
typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

typedef struct s_env
{
	t_env_var	*variables;
	int			nbr_var_env;
	ssize_t		last_exit_status;
}	t_env;

/* Main shell structure */
typedef struct s_shell
{
	t_env	*env;
	t_cmd	*commands;
	char	*input_line;
	int		interactive;
	int		running;		//flag a utiliser? pour exit -> exit()
}	t_shell;

typedef struct s_exec
{
	t_shell	*shell;
	t_cmd	*current_cmd;
	int		*pipe_fds;
	char	*path;
	char	**env_copy;
	int		nb_arg;
	int		is_pipe;
	int		saved_stdin;
	int		saved_stdout;
	int		saved_stderr;
	int		nb_var_env;
	int		nbr_pipes;
	int		nb_process;
}	t_exec;

typedef struct s_builtin
{
	char	*builtin;
	int		(*fonction)(t_exec *exec);
}	t_builtin;

/* ************************************************************************** */
/*                            GLOBAL VARIABLE                                 */
/* ************************************************************************** */

/* Global variable for signal handling (only signal number allowed) */
extern int	g_signal_received;

/* ************************************************************************** */
/*                            FUNCTION PROTOTYPES                             */
/* ************************************************************************** */
/* Cleanup */
void		cleanup_iteration(t_shell *shell);
void		cleanup_all(t_exec *exec);
void		cleanup_shell(t_shell *shell);
void		cleanup_and_exit(int exit_code);

/* Init */
t_shell		*init_shell(char **envp);
void		init_minimal_env_vars(t_env *env);

/* Shell_loop */
void		process_input(t_shell *shell);
int			handle_iteration(t_shell *shell);
void		run_shell_loop(t_shell *shell);

/* ============================= BUILTINS ================================== */
/* CD */
int			builtin_cd(t_exec *exec);

/* Echo */
int			builtin_echo(t_exec *exec);

/* Env */
int			builtin_env(t_exec *exec);

/* Exit utils */
int			check_limit(char *str);

/* Exit */
int			builtin_exit(t_exec *exec);

/* Export assign */
int			export_with_assignment(t_exec *exec);

/* Export no args*/
void		print_env_sorted(t_exec *exec);

/* Export */
int			is_a_valid_identifier(char *str);
int			builtin_export(t_exec *exec);

/* PWD*/
int			update_pwd(t_exec *exec);
int			update_oldpwd(t_exec *exec);
int			builtin_pwd(t_exec *exec);

/* Unset */
int			builtin_unset(t_exec *exec);

/* builtins */
int			is_builtin(t_cmd *cmd);
int			execute_builtin(t_cmd *cmd, t_exec *exec);
/* ==================================== DEBUG ============================== */
/*Debug commands */
void		print_commands(t_cmd *commands);
void		print_commands_expanded(t_cmd *commands);

/*Debug tokens */
void		print_tokens(t_token *tokens);

/* ============================= ENVIRONMENT ============================== */
/* Env_get */
char		*get_env_var(char *key, t_env *env);

/* Env_set */
int			set_env_var(char *key, char *new_value, t_env *env, int is_append);

/* Env_utils */
t_env		*init_environment(char **envp);
void		free_environment(t_env *env);
char		**env_to_string(t_env *env);

/* Env_var_utils */
t_env_var	*create_env_var(char *key, char *value);
void		add_env_var(t_env_var **head, t_env_var *new_var);
int			remove_env_var(char *key, t_env *env);
void		free_env_var(t_env_var *var);
void		free_env_vars(t_env_var *vars);

/* Shell_lvl */
int			ft_shlvl(t_shell *shell);

/* ============================= EXECUTION ================================== */
/* Exec_externe */
int			execute_externe(t_cmd *cmd, t_exec *exec);

/* Exec_single_cmd */
int			execute_direct(t_cmd *cmd, t_exec *exec);
void		execute_single_command(t_cmd *cmd, t_exec *exec);

/* Exec_utils */
t_exec		*create_exec(t_shell *shell);
void		init_exec(t_exec *exec);
void		free_exec(t_exec *exec);
void		free_var(t_exec *exec);

/* Exec */
void		execute_commands(t_shell *shell);

/* Pipe_utils */
void		kill_all_process(pid_t *pids, int count);
int			create_pipes(t_exec *exec, int ***pipes);
int			free_pipes(int **pipes, t_exec *exec);
void		close_pipes(int **pipes, t_exec *exec);

/* Pipe */
void		handle_pipeline(t_cmd *cmd, t_exec *exec);

/* Heredoc */
t_heredoc	*create_heredoc(char *delimiter, int quoted);
int			handle_heredoc(char *delimiter);
void		free_heredoc(t_heredoc *heredoc);

/* Only redir */
void		handle_redirection_only(t_cmd *cmd);

/* Redir */
int			apply_redirections(t_cmd *cmd);
int			has_redirections(t_cmd *commands);

/* ============================= EXPAND ==================================== */
/* Expand_utils */
void		expand_assignments(t_ass *assignments, t_shell *shell);
void		expand_redirections(t_redir *redirections, t_shell *shell);
void		expand_command_args(t_cmd *cmd, t_shell *shell);

/* Expand */
void		expand_commands(t_shell *shell);

/* Quotes helpers */
char		*process_unquoted_char(char *result, char *str,
				int *index, t_shell *shell);
char		*extract_unquoted_section(char *str, int *index);
char		*process_quoted_section(char *result, char *str,
				int *index, t_shell *shell);
int			single_quote(char *str);


/* Quotes utilities */
int			find_matching_quote(char *str, int start);
char		*append_char_to_str(char *str, char c);
char		*join_and_free(char *str1, char *str2);
int			is_valid_var_char(char c);
int			has_quotes(char *str);

/* Quotes*/
char		*extract_var_name(char *str, int *index);
char		*expand_variables(char *str, t_shell *shell);
char		*handle_quotes(char *str, t_shell *shell);
int			check_quotes(char *str);

/* ============================= LEXER ===================================== */
/* Lexer utilities */
int			is_operator(char c);
int			is_whitespace(char c);
int			skip_whitespace(char *input, int i);
char		*extract_word(char *input, int start, int *end);
char		*extract_operator(char *input, int start, int *end);

/* Lexer */
t_token		*tokenize(char *input);

/* Token utilities */
t_token		*create_token(char *value, int type);
void		add_token(t_token **head, t_token *new_token);
void		free_tokens(t_token *tokens);

/* ============================= PARSER ==================================== */
/* Check token syntax */
int			check_token_syntax(t_token *tokens);

/* Parser args */
int			extract_arguments(t_token *tokens, t_cmd *cmd);

/* Parser assignements */
int			extract_assignments(t_token *tokens, t_cmd *cmd);

/* Parser redir */
int			extract_redirections(t_token *tokens, t_cmd *cmd);

/* Parser utils */
int			count_words(t_token *tokens);

/* Paser */
t_cmd		*parse_tokens(t_token *tokens);

/* ============================= SIGNALS ================================== */
/* Child */
void		heredoc_child_signal(void);
void		child_signal(void);
void		handler_child_sigint(int sig);

/* Parent */
void		parent_signal(void);
void		heredoc_parent_signal(void);
void		sig_core_dump_parent_signal(void);

/* Signals */
void		handle_signal(void);
void		handle_sigint(int sig);
void		handle_sigquit(int sig);
void		handle_signal_heredoc(int sig);

/* ============================= STRUCTURE ================================ */
/* Append assignement */
t_ass		*create_append_assignment(char *key, char *value);
int			is_append_assignment_word(const char *word);
int			split_append_assignment(const char *assignment,
				char **key, char **value);

/* Assignement */
t_ass		*create_assignment(char *key, char *value);
void		add_assignment(t_ass **head, t_ass *new_assign);
void		free_assignments(t_ass *assignments);
int			is_assignment_word(char *str);
int			split_assignment(char *assignment, char **key, char **value);

/* t_cmd utils */
t_cmd		*create_command(void);
void		add_command(t_cmd **head, t_cmd *new_cmd);
void		free_commands(t_cmd *commands);
int			get_nb_commands(t_cmd *commands);

/* t_redir utils */
t_redir		*create_redirection(int type, char *target);
void		add_redirection(t_redir **head, t_redir *new_redir);
void		free_redirections(t_redir *redirections);

/* ============================= UTILS ===================================== */
/* check_args */
int			check_args(t_exec *exec);

/* cmd_args_utils */
int			get_nb_command_args(t_cmd *commands);

/* cmd_check */
int			apply_cmd_path(t_cmd *cmd, t_exec *exec);

/* Cmd_utils */
int			update_state_path(t_cmd *cmd);
int			check_command_exist(char *name);

/* Error */
void		print_syntax_error(char *token);
void		print_error(char *cmd, char *arg, char *msg);

/* Fd_utils */
int			open_and_dup(const char *file, int flags, int stdfd);
int			dup_and_close(int fd, int stdfd, const char *err);
void		restore_std(int saved_stdout, int saved_stdin);

/* Malloc */
void		*safe_malloc(size_t size);

/* Path_utils*/
char		*get_path(t_env *env);
void		update_var_path(t_exec *exec);

/* Shell_utils */
void		set_exit_status(int exit_value);
void		print_welcome(void);
t_shell		*get_shell(char **envp);

/* utils */
int			ft_3d_len(char ***tab_3d);
void		free_array(char **array);

#endif
