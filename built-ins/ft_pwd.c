/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <tkurukul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:47:13 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/16 17:14:06 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_pwd(t_info *info)
{
	char	*buf; //THE-PATH_MAX-ON-LINUX

	buf = getcwd(NULL, 0); // AUTOMATICALLY ALLOCATES MEMORY NESSCARY FOR  THE PATH
	if (!buf)
	{
		ft_printf(2, "pwd: error while retrieving current directory: getcwd:");
		return (estat(1, info), NULL);
	}
	return (estat(0, info), buf);
}

void	print_pwd(t_info *info)
{
	char	*pwd;

	pwd = ft_pwd(info);
	ft_printf(1, "%s\n", pwd);
	free(pwd);
}

/* int	main()
{
	char	*hh;

	hh = ft_pwd();
	printf("%s\n", hh);
	free(hh);
} */
