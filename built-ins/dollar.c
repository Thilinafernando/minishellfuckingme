/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <tkurukul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:47:35 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/16 17:36:33 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// go inside double quotes and single quotes

#include "minishell.h"

char	*setfull(char *gg, char *str, int size)
{
	char	*tmp;
	int		len;
	char	*full;

	len = ft_strlen(str) - size;
	tmp = malloc((len + 1) * sizeof(char));
	if (!tmp)
		return (NULL);
	ft_strncpy(tmp, str + size, len);
	tmp[len] = '\0';
	full = ft_strjoin(gg, tmp);
	free(tmp);
	return (full);
}

char	*mdollar(char *str, t_info *info)
{
	char	*line;
	int		i;
	int		len;

	len = ft_strlen(str);
	i = 0;
	if (str[0] == '?')
		return (ft_itoa(info->exit_status));
	else if (str[0] >= '0' && str[0] <= '9')
		return (ft_strdup(str + 1));
	while (info->env[i])
	{
		if (ft_strncmp(info->env[i], str, len) == 0 && info->env[i][len] == '=')
		{
			line = ft_strdup(info->env[i] + (len + 1));
			return (line);
		}
		i++;
	}
	return (NULL);
}
