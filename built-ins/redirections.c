/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <tkurukul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 22:04:01 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/20 23:06:55 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// if open fails, estat to the right number

#include "minishell.h"

int	ft_input(char **exec, t_info *info)
{
	int	fd;

	fd = open(exec[1], O_RDONLY);
	if (fd == -1)
	{
		ft_printf(2., "Minishell : %s: %s\n", exec[1], strerror(errno));
		if (errno == ENOENT)
			return (estat(127 , info), -1);
		else if (errno == EACCES)
			return (estat(126 , info), -1);
		else
			return (estat(1 , info), -1);
	}
	info->fd_in_child = fd;
	return (0);
}

int	ft_output(char **exec, t_info *info)
{
	int	fd;

	fd = open(exec[1], O_WRONLY | O_CREAT| O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_printf(2., "Minishell : %s: %s\n", exec[1], strerror(errno));
		if (errno == ENOENT)
			return (estat(127 , info), -1);
		else if (errno == EACCES)
			return (estat(126 , info), -1);
		else
			return (estat(1 , info), -1);
	}
	info->fd_out_child = fd;
	return (0);
}

int	ft_append(char **exec, t_info *info)
{
	int	fd;

	fd = open(exec[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_printf(2., "Minishell : %s: %s\n", exec[1], strerror(errno));
		if (errno == ENOENT)
			return (estat(127 , info), -1);
		else if (errno == EACCES)
			return (estat(126 , info), -1);
		else
			return (estat(1 , info), -1);
	}
	info->fd_out_child = fd;
	return (0);
}

char	*heredoc_filename(void)
{
	char	*pid;
	char	*filename;
	char	*tmp;

	pid = ft_itoa(getpid());
	if (!pid)
		return (NULL);
	tmp = ft_strjoin("/tmp/.heredoc_", pid);
	free(pid);
	if (!tmp)
		return (NULL);
	filename = ft_strjoin(tmp, ".tmp");
	free(tmp);
	return (filename);
}



void	ft_heredoc_process(char **exec, t_info *info, char *filename)
{
	char	*str;
	char	*dollar;
	int		fd;
	int		std_in;


	dollar = NULL;
	std_in = open("/dev/tty", O_RDONLY);
	if (std_in == -1)
	{
		ft_printf(2, "Minishell: cannot open /dev/tty\n");
		return (free(filename), exit(1));
	}
	if (dup2(std_in, STDIN_FILENO) == -1)
	{
		ft_printf(2, "Minishell: dup2 failed\n");
		close(std_in);
		free(filename);
		exit(1);
	}
	close(std_in);
	fd = open(filename, O_CREAT| O_TRUNC | O_WRONLY, 0644);
	if (fd == -1)
	{
		ft_printf(2, "Minishell : %s: %s\n", filename, strerror(errno));
		return (free(filename), exit(126));
	}
	while (1)
	{
		str = readline("> ");
		if (!str || !exec[1] || ft_strcmp(str, exec[1]) == 0)
		{
			if (!str && signal_status != 130)
				ft_printf(2, "Minishell: warning: here-document delimited by end-of-file (wanted %s)\n", exec[1]);
			free(str);
			free(filename);
			close(fd);
			free_all(info);
			if (signal_status == 130)
				exit(130);
			exit(0);
		}
		dollar = expand_dollar1(str, info);
		if (dollar)
		{
			ft_printf(fd, "%s\n", dollar);
			free(dollar);
		}
		else
			ft_printf(fd,"%s\n", str);
		free(str);
	}
}

int	ft_heredoc_parent(int pid,int *status, t_info *info)
{
	int	fd;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, status, 0);
	fd = open (info->heredoc, O_RDONLY);
	if (fd == -1)
	{
		ft_printf(2, "Minishell: %s: %s\n", info->heredoc, strerror(errno));
		return (estat(126, info), -1);
	}
	unlink(info->heredoc);
	free(info->heredoc);
	info->heredoc = NULL;
	info->fd_in_child = fd;
	set_signals();
	if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGINT)
		{
			write(1, "\n", 1);
			sstat(130);
			return (-1);
		}
		if (WTERMSIG(*status) == SIGQUIT)
		{
			ft_printf(1, "Quit (core dumped)\n");
			sstat(131);
			return (-1);
		}
	}
	return (estat(0, info), 0);
}

int	ft_heredoc(char **exec, t_info *info)
{
	int		status;
	pid_t	pid;
	char	*filename;

	filename = heredoc_filename();
	if (!filename)
		return (ft_printf(2, "Minishell: failed to create heredoc filename\n"), -1);
	info->heredoc = filename;
	pid = fork();
	if (pid == -1)
		return (free(filename), -1);
	if (pid == 0)
	{
		signal(SIGINT, ctrl_c_here);
		signal(SIGQUIT, SIG_IGN);
		ft_heredoc_process(exec, info, filename);
	}
	else
	{
		if (ft_heredoc_parent(pid, &status, info) == -1)
			return (-1);
	}
	return (0);
}
