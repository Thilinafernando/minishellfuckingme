/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <tkurukul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:29:40 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/22 17:36:02 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// while that createws forks for every process.; the parent will wait for the child and start the next loop;
// first and last have to have their unused pipes closed in normal use(without redirections).
// redirections have to be exectued after opening fork and before every exectution of command


// intergrate built ins
// in parent after each

#include "minishell.h"

// int	middle_child(int pipefd[2])
// {
// 	if (dup2(pipefd[1], 1) == -1)
// 	{
// 		close_fd(pipefd);
// 		return (-1);
// 	}
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	return (0);
// }
void	print_matrix(char **matrix)
{
	int	i = 0;

	if (!matrix)
	{
		ft_printf(1, "Matrix is NULL\n");
		return;
	}
	while (matrix[i])
	{
		ft_printf(1, "[%d]: %s\n", i, matrix[i]);
		i++;
	}
	fflush(stdin);
}

void	one_exec(char **command, t_info *info, int fd[2])
{
	char	*str;

	str = NULL;
	str = abs_path(command[0], info);
	if (is_directory(str) == 1)
	{
		free(str);
		close_fd(fd);
		free_all(info);
		exit(126);
	}
	if (!str)
	{
		failure_command(fd, command, info);
		free_all(info);
		exit (info->exit_status);
	}
	execve(str, command, info->env);
	failure(fd, info);
	free_all(info);
	free(str);
	exit (126);
}



int	ft_redirections(char **matrix, t_info *info)
{
	int	result;

	result = 0;
	if (ft_strcmp(matrix[0],"<") == 0)
		result = ft_input(matrix, info);
	else if (ft_strcmp(matrix[0],">") == 0)
		result = ft_output(matrix, info);
	else if (ft_strcmp(matrix[0],">>") == 0)
		result = ft_append(matrix, info);
	else if (ft_strcmp(matrix[0],"<<") == 0)
		result = ft_heredoc(matrix, info);
	if (result == -1)
		return (-1);
	return (0);
}

int is_redirection(char **matrix)
{
	if (!matrix || !matrix[0])
		return 0;
	return (ft_strcmp(matrix[0], "<") == 0
		|| ft_strcmp(matrix[0], ">") == 0
		|| ft_strcmp(matrix[0], ">>") == 0
		|| ft_strcmp(matrix[0], "<<") == 0);
}

int is_builtin(char **matrix)
{
	if (!matrix || !matrix[0])
		return (0);
	if (ft_strcmp(matrix[0], "cd") == 0
		|| ft_strcmp(matrix[0], "pwd") == 0
		|| ft_strcmp(matrix[0], "unset") == 0
		|| ft_strcmp(matrix[0], "export") == 0
		|| ft_strcmp(matrix[0], "env") == 0
		|| ft_strcmp(matrix[0], "echo") == 0
		|| ft_strcmp(matrix[0], "exit") == 0)
		return (1);
	return (0);
}

void exec_builtin(char **matrix, t_info *info)
{
	if (ft_strcmp(matrix[0], "cd") == 0)
		ft_cd(matrix, info);
	else if (ft_strcmp(matrix[0], "pwd") == 0)
		print_pwd(info);
	else if (ft_strcmp(matrix[0], "unset") == 0)
		ft_unset(info, matrix);
	else if (ft_strcmp(matrix[0], "export") == 0)
		ft_export(info, matrix);
	else if (ft_strcmp(matrix[0], "env") == 0)
		ft_env(matrix ,info);
	else if (ft_strcmp(matrix[0], "echo") == 0)
		ft_echo(matrix, info);
	else if (ft_strcmp(matrix[0], "exit") == 0)
		ft_pipe_exit(matrix, info);
}

int	istt_builtin(char ***matrix, t_info *info)
{
	int	i;
	int	mat;

	i = 0;
	mat = 0;
	while(is_redirection(matrix[mat]))
		mat++;
	if (is_builtin(matrix[mat]) == 1)
	{
		mat = 0;
		while(is_redirection(matrix[mat]))
		{
			if (ft_redirections(matrix[mat], info) == -1)
				return (-2);
			if (if_in(matrix, mat) == 1 && info->fd_in_child != -420)
			{
				close(info->fd_in_child);
				info->fd_in_child = -420;
			}
			if (if_out(matrix, mat) == 1 && info->fd_out_child != -420)
			{
				close(info->fd_out_child);
				info->fd_out_child = -420;
			}
			mat++;
		}
		ft_remove(matrix);
		if (ft_strcmp(matrix[mat][0], "cd") == 0)
			ft_cd(matrix[mat], info);
		else if (ft_strcmp(matrix[mat][0], "pwd") == 0)
			print_pwd(info);
		else if (ft_strcmp(matrix[mat][0], "unset") == 0)
			ft_unset(info, matrix[mat]);
		else if (ft_strcmp(matrix[mat][0], "export") == 0)
			ft_export(info, matrix[mat]);
		else if (ft_strcmp(matrix[mat][0], "env") == 0)
			ft_env(matrix[mat], info);
		else if (ft_strcmp(matrix[mat][0], "echo") == 0)
			ft_echo(matrix[mat], info);
		else if (ft_strcmp(matrix[mat][0], "exit") == 0)
			ft_exit(matrix[mat], info);
		else
			i = -1;
	}
	else
			i = -1;
	return (i);
}

int	is_only_redirection(char ***matrix)
{
	int	i;

	i = 0;
	while (matrix[i] && matrix[i][0][0] != '|')
	{
		if (!(matrix[i][0]) ||
			!(ft_strcmp(matrix[i][0], "<") == 0 ||
			  ft_strcmp(matrix[i][0], ">") == 0 ||
			  ft_strcmp(matrix[i][0], "<<") == 0 ||
			  ft_strcmp(matrix[i][0], ">>") == 0))
			return (-1);
		i++;
	}
	return (0);
}

int	count_exec_blocks(char ***exec)
{
	int	i = 0;
	int	count = 0;
	int	has_cmd_or_redir = 0;

	while (exec[i])
	{
		if (!exec[i][0])
		{
			i++;
			continue;
		}
		if (exec[i][0][0] == '|')
		{
			if (has_cmd_or_redir)
				count++;
			has_cmd_or_redir = 0;
		}
		else if (ft_isalpha(exec[i][0][0]) || ft_isascii(exec[i][0][0]) ||is_redirection(exec[i]))
			has_cmd_or_redir = 1;
		i++;
	}
	if (has_cmd_or_redir)
		count++;
	return (count);
}


void	ft_execution(t_info *info)
{
	int	i;
	int	j;
	int	mat;
	int	flag;
	int	count;
	int	cpipe[2];
	int	prevpipe;
	pid_t	pid;
	pid_t	pids[64];
	int	pid_counts;
	int		status;

	i = 0;
	cpipe[0] = -1;
	cpipe[1] = -1;
	info->fd_in_out[0] = dup(STDIN_FILENO);
	ft_printf(1, "in; %d\n", info->fd_in_out[0]);
	info->fd_in_out[1] = dup(STDOUT_FILENO);
	ft_printf(1, "out; %d\n", info->fd_in_out[1]);
	count = count_exec_blocks(info->exec);
	printf("count : %d\n", count);
	pid_counts = 0;
	prevpipe = -42;
	mat = 0;
	flag = 0;
	while (i < count)
	{
		if (is_only_redirection(info->exec) == 0)
		{
			while(info->exec[mat])
			{
				ft_redirections(info->exec[mat], info);
				mat++;
				if (info->fd_in_child != -420)
					close(info->fd_in_child);
				if (info->fd_out_child != -420)
					close(info->fd_out_child);
				info->fd_in_child = -420;
				info->fd_out_child = -420;
			}
			break;
		}
		if (count == 1)
		{
			flag = istt_builtin(info->exec, info);
			if (flag == -2)
			{
				ft_refresh_fd(info);
				break;
			}
			if (flag == 0)
				break;
		}
		if (i != (count - 1))
		{
			if (pipe(cpipe) == -1)
			{
				ft_printf(2, "Minishell: error pipe\n");
					return (estat(1, info));
			}
		}
		flag = 0;
		while (is_redirection(info->exec[mat]) && info->exec[mat][0][0] != '|')
		{
			if (ft_redirections(info->exec[mat], info) == -1)
			{
				flag = 1;
				break;
			}
			if (if_in(info->exec, mat) == 1 && info->fd_in_child != -420)
			{
				close(info->fd_in_child);
				info->fd_in_child = -420;
			}
			if (if_out(info->exec, mat) == 1 && info->fd_out_child != -420)
			{
				close(info->fd_out_child);
				info->fd_out_child = -420;
			}
			mat++;
		}
		if (flag == 1)
		{
			while (info->exec[mat] && info->exec[mat][0][0] != '|')
				mat++;
			mat++;
			i++;
			continue;
		}
		ft_remove(info->exec);
		if (!info->exec[mat] || info->exec[mat][0][0] == '|')
				break ;
		pid = fork();
		if (pid == -1)
		{
			ft_printf(2, "Minishell: error fork");
			exit(1);
		}
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (i != 0 && prevpipe != -42)
			{
				if (dup2(prevpipe, 0) == -1)
					return (ft_printf(2, "Minishell: error dup2\n"), exit(1));
				close(prevpipe);
			}
			if (i != (count - 1))
			{
				close(cpipe[0]);
				if (dup2(cpipe[1], 1) == -1)
					return (ft_printf(2, "Minishell: error dup2\n"), exit(1));
				close(cpipe[1]);
			}
			if (info->fd_in_child != -420)
			{
				if (dup2(info->fd_in_child, 0) == -1)
					return (ft_printf(2, "Minishell: error dup2\n"), exit(1));
				close(info->fd_in_child);
			}
			if (info->fd_out_child != -420)
			{
				if (dup2(info->fd_out_child, 1) == -1)
					return (ft_printf(2, "Minishell: error dup2\n"), exit(1));
				close(info->fd_out_child);
			}
			if (is_builtin(info->exec[mat]))
			{
				exec_builtin(info->exec[mat], info);
				free_all(info);
				exit(info->exit_status);
			}
			else
			{
				close(info->fd_in_out[0]);
				close(info->fd_in_out[1]);
				one_exec(info->exec[mat], info, cpipe);
			}
		}
		else
		{
			pids[pid_counts++] = pid;
			while (info->exec[mat])
			{
				if (info->exec[mat][0][0] == '|')
				{
					mat++;
					break;
				}
				mat++;
			}
			if (prevpipe != -42)
				close(prevpipe);
			if (i != (count -1))
			{
				close(cpipe[1]);
				prevpipe = cpipe[0];
			}
			if (info->fd_in_child != -420)
				close(info->fd_in_child);
			if (info->fd_out_child != -420)
				close(info->fd_out_child);
			info->fd_in_child = -420;
			info->fd_out_child = -420;
		}
		i++;
	}
	j = 0;
	while (j < pid_counts)
	{
		if (waitpid(pids[j], &status, 0) == -1)
		{
			ft_printf(2, "Minishell: error waitpid\n");
			free3(info->exec);
			exit(1);
		}
		if (j == (pid_counts - 1))
		{
			if (WIFEXITED(status))
				info->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGQUIT)
					ft_printf(2, "Quit (core dumped)\n");
				info->exit_status = 128 + WTERMSIG(status);
			}
		}
		j++;
	}
	ft_refresh_fd(info);
	free3(info->exec);
	close_fd(info->fd_in_out);
}


