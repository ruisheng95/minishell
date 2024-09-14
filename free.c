/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 00:15:18 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/14 03:41:51 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_t_tokens(t_tokens *obj)
{
	t_tokens	*node;
	t_tokens	*temp_node;

	if (!obj)
		return ;
	node = obj;
	while (node)
	{
		temp_node = node;
		node = node->next;
		free(temp_node);
	}
}

void	free_t_node(t_node *obj)
{
	t_node	*node;
	t_node	*temp_node;
	int		i;

	if (!obj)
		return ;
	node = obj;
	while (node)
	{
		temp_node = node;
		node = node->next;
		if (temp_node->type == s_command)
		{
			free(temp_node->simple_cmd.cmd);
			i = -1;
			while (temp_node->simple_cmd.array[++i])
				free(temp_node->simple_cmd.array[i]);
			free(temp_node->simple_cmd.array);
		}
		free(temp_node);
	}
}

void	free_t_cmd_list(t_cmd_list *obj)
{
	t_cmd_list	*node;
	t_cmd_list	*temp_node;

	if (!obj)
		return ;
	node = obj;
	while (node)
	{
		temp_node = node;
		node = node->next;
		if (ft_strcmp(temp_node->cmd[0], heredoc_cmd) == 0)
		{
			free(temp_node->cmd[0]);
			free(temp_node->cmd);
		}
		free(temp_node);
	}
}

void	free_2d_array(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}
