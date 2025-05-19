/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <tkurukul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:09:42 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/05/19 22:55:08 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "../libprintf/ft_printf.h"
# include "../get_next_line/get_next_line.h"

# include <stdlib.h>
#include <sys/wait.h>
# include <errno.h>
# include <unistd.h>
#include <dirent.h>
# include <stdio.h>
# include <string.h>
# include <signal.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

// assign it to 0 in main, and change exit status when command is exectued
extern int signal_status;

typedef struct s_env {
	char	*name;
	char	*content;
	struct s_env	*next;
}t_env;

typedef struct s_info {
	char	***exec;
	int		pos;
	char	**env;
	char	**tmp;
	int		size;
	// char	*oldpwd;
	int		fd_in_out[2];
	int		exit_status;
	int		fd_in_child;
	int		fd_out_child;
	char	*heredoc;
} t_info;

typedef enum {
	TOKEN_WORD,       // Regular words/commands
	TOKEN_PIPE,       // |
	TOKEN_REDIR_IN,   // <
	TOKEN_REDIR_OUT,  // >
	TOKEN_REDIR_APPEND, // >>
	TOKEN_SQUOTE, // single quosted string (no expansion)
	TOKEN_DQUOTE, // double quoted string (allow expansion)
	TOKEN_HEREDOC,    // <<
	TOKEN_VAR,
	TOKEN_SPACE       // Space characters (optional)
} t_token_type;

typedef struct s_token {
	char *content;         // The actual text
	t_token_type type;     // Type of token
	struct s_token *next;  // Pointer to next token
} t_token;


// built-ins / handle malloc fails and general errors
char	*ft_pwd(t_info *info);
void	free_mat(char **matrix);
int		exisit(char **matrix, char *arg);
void	matrix_tmp(t_info *info);
int		ft_strcmp(const char *s1, const char *s2);
int		verify_equal(char *str);
void	form_env(char **env, t_info	*info);
void	print_pwd(t_info *info);
void	ft_unset(t_info *info, char **args);
void	ft_export(t_info *info, char **args);
void	ft_env(char **matrix, t_info *info);
void	ft_cd(char **args, t_info *info);
void	ft_echo(char **args, t_info *info);
void	ft_exit(char **args, t_info *info);

// redirections
int	ft_input(char **exec, t_info *info);
int	ft_output(char **exec, t_info *info);
int	ft_append(char **exec, t_info *info);
int	ft_heredoc(char **exec, t_info *info);
void	ctrl_c_here(int sig);

// dollar
/* char	*dollarfull(char *str, t_info *info);
int	arg_execve(char ***command, t_info *info); */
char	*mdollar(char *str, t_info *info);
char	*expand_dollar1(char *line, t_info *info);



// pipe and proccess
void	ft_execution(t_info *info);
void	close_fd(int *ar);
void		failure(int fd[2], t_info *info);
void		failure_command(int fd[2], char **str, t_info *info);
char	*abs_path(char *command, t_info *info);
char	*build_full(char *path, char *command);
char	**find_path(char **envp);
int		is_directory(const char *path);

// signals
void	estat(int i, t_info *info);
void	sstat(int i);
void	ctrl_c(int sig);
void	set_signals(void);

// intergration
void	save_redirections(t_info *info, t_token *token);
void	save_command(t_info *info, t_token **token);
void	save_other(t_info *info, t_token **token);
void	ft_conditions(t_info *info, t_token **token);
void	form_main(t_token *token, t_info *info);

// test functions
void ft_listadd_back(t_token **lst, t_token *new);
t_token *ft_newnode(char *content, t_token_type type);
char	*ft_strncpy(char *dest, char *src, unsigned int n);
void	ft_refresh_fd(t_info *info);
int		if_out(char ***exec, int mat);
int		if_in(char ***exec, int mat);
void	ft_remove(char ***matrix);



int is_builtin(char **matrix);

// free
void	free3(char ***matrix);
void  free_all(t_info *info);
//parssing
t_token	*free_tokens(t_token *head);
t_token	*creat_token(char *content, t_token_type type);
t_token	*get_last_token(t_token *head);
t_token	*handle_env_variable(char *input, int *i, t_token *head, t_token **current_word_token, t_info *info);
t_token	*handle_single_quotes(char *input, int *i, t_token *head, t_token *current);
t_token	*add_double_quote_text(char *input, int start, int end, t_token *head, t_token *current);
t_token	*process_double_quotes(char *input, int *i, t_token *head, t_token *current, t_info *info);
t_token	*handle_quotes(char *input, int *i, t_token *head, t_token *current, t_info *info);
int		validate_syntax(t_token *tokens, t_info *info);
t_token *join_word_segment(char *segment, t_token *head, t_token **current_word_token);
char *token_type_to_string(t_token_type token_type);
////try//
t_token *expand_and_tokenize_var(char *var_value, t_info *info);
t_token *tokenize_string(char *str, t_info *info);
t_token	*handle_redirection(char *input, int *i, t_token *head, t_token *current);
char *extract_single_quote_content(char *input, int *i);
char *extract_word_segment(char *input, int *i);
char *extract_double_quote_content(char *input, int *i, t_info *info);
t_token	*handle_pipe(int *i, t_token *head, t_token *current);
t_token *join_word_segment_2(char *segment, t_token *head, t_token **current_word_token);
t_token *join_word_segment_3(char *segment, t_token *head, t_token **current_word_token);

#endif
