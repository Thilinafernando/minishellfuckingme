/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <tkurukul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 19:48:49 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/21 00:09:50 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// form_env should be in the main with the read line

void	form_env(char **env, t_info	*info)
{
	int		count;

	count = 0;
	while (env[count])
		count++;
	info->env = malloc((count + 1) * sizeof(char *));
	if (!info->env)
	{
		ft_printf(2, "ERROR WHILE COPYING ENV\n");
		return ;
	}
	count = 0;
	while(env[count])
	{
		if (ft_strncmp(env[count], "_=", 2) == 0)
			info->env[count] = ft_strdup("_=/built-ins/ft_env");
		else
			info->env[count] = ft_strdup(env[count]);
		// if (ft_strncmp(env[count], "OLDPWD=", 7) == 0)
			// info->oldpwd = ft_strdup(info->env[count] + 7);
		count++;
	}
	info->env[count] = NULL;
}

void	ft_env(char **matrix, t_info *info)
{
	int	i;

	if (matrix[1])
	{
		ft_printf(2, "Minishell: env: '%s': No arguments allowed\n", matrix[1]);
		estat(2, info);
		return ;
	}
	i = 0;
	while (info->env[i])
	{
		if (verify_equal(info->env[i]) != -1)
			ft_printf(STDOUT_FILENO, "%s\n", info->env[i]);
		i++;
	}
	return (estat(0, info));
}
/* int	main(int ac, char **av, char **env)
{
	int	i;
	t_info	i;

	(void)ac;
	(void)av;

	i = 0;
	i.matrix = ft_env(env);
	while (matrix[i])
	{
		printf("%s\n", i.matrix[i]);
		fflush(stdout);
		free(i.matrix[i]);
		i++;
	}
	free(i.matrix);
} */
