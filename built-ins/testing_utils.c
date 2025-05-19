/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <tkurukul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:36:36 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/19 23:56:40 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>

t_token *ft_newnode(char *content, t_token_type type)
{
    t_token *node;

    node = malloc(sizeof(t_token));
    if (!node)
        return (NULL);
    node->content = content; // Assume content is already duplicated if needed
    node->type = type;
    node->next = NULL;
    return (node);
}

void ft_listadd_back(t_token **lst, t_token *new)
{
    t_token *tmp;

    if (!lst || !new)
        return;
    if (!*lst)
    {
        *lst = new;
        return;
    }
    tmp = *lst;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	b;

	b = 0;
	while (b < n && src[b] != '\0')
	{
		dest[b] = src[b];
		b++;
	}
	while (b < n)
	{
		dest[b] = '\0';
		b++;
	}
	return (dest);
}

void	free_mat(char **matrix)
{
	int	i;

	i = 0;
	if (matrix == NULL)
		return ;
	while (matrix[i])
		i++;
	i--;
	while (i >= 0 && matrix[i])
	{
		free(matrix[i]);
		i--;
	}
    free(matrix);
	matrix = NULL;
	return ;
}

int		if_in(char ***exec, int mat)
{
	mat++;
	while (exec[mat] && exec[mat][0][0] != '|')
	{
		if (ft_strcmp(exec[mat][0], "<") == 0
		|| ft_strcmp(exec[mat][0], "<<") == 0)
			return (1);
		mat++;
	}
	return (0);
}

int		if_out(char ***exec, int mat)
{
	mat++;
	while (exec[mat] && exec[mat][0][0] != '|')
	{
		if (ft_strcmp(exec[mat][0], ">") == 0
		|| ft_strcmp(exec[mat][0], ">>") == 0)
			return (1);
		mat++;
	}
	return (0);
}

void	ft_remove(char ***matrix)
{
	int	i;
	int j;
	char	*tmp;

	i = 0;
	while (matrix[i])
	{
		j = 0;
		while (matrix[i][j])
		{
			if (ft_strcmp(matrix[i][j], ";<") == 0 ||
				ft_strcmp(matrix[i][j], ";>") == 0 ||
				ft_strcmp(matrix[i][j], ";<<") == 0 ||
				ft_strcmp(matrix[i][j], ";>>") == 0)
			{
				tmp = ft_strdup(matrix[i][j] + 1);
				free(matrix[i][j]);
				matrix[i][j] = tmp;
			}
			// ft_printf(1, "%s\n", matrix[i][j]);
			j++;
		}
		i++;
	}
}
