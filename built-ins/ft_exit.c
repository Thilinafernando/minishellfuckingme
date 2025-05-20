/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <tkurukul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:23:50 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/21 00:41:13 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_num(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i] && (str[0] == '-' || str[0] == '+'))
		return (-1);
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			i++;
		else
			return (-1);
	}
	return (0);
}

void	ft_exit(char **args, t_info *info)
{
	int	num;

	ft_printf(1, "exit\n");
	if (args[1])
	{
		if (is_num(args[1]) == -1 && !args[2])
		{
			ft_printf(2, "Minishell: exit: %s: numeric argument required\n", args[1]);
			free_all(info);
			exit(2);
		}
		if (args[2])
		{
			ft_printf(2,"Minishell: exit: too many arguments\n");
			return (estat(2, info));
		}
		num = ft_atoi(args[1]);
		if (num < 0)
			info->exit_status = 256 + num;
		else
			info->exit_status = num;
	}
	free_all(info);
	exit(info->exit_status);
}

void	ft_pipe_exit(char **args, t_info *info)
{
	int	num;

	if (args[1])
	{
		if (is_num(args[1]) == -1 && !args[2])
		{
			ft_printf(2, "Minishell: exit: %s: numeric argument required\n", args[1]);
			free_all(info);
			exit(2);
		}
		if (args[2])
		{
			ft_printf(2,"Minishell: exit: too many arguments\n");
			return (estat(2, info));
		}
		num = ft_atoi(args[1]);
		if (num < 0)
			info->exit_status = 256 + num;
		else
			info->exit_status = num;
	}
	free_all(info);
	exit(info->exit_status);
}
