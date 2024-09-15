/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rng <rng@student.42kl.edu.my>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 00:15:18 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/15 22:05:16 by rng              ###   ########.fr       */
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
		if (temp_node->type == COMMAND || temp_node->type == ARGUMENTS)
			free(temp_node->token);
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
		if (temp_node->type == S_COMMAND)
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
		if (temp_node->cmd && ft_strcmp(temp_node->cmd[0], HEREDOC_CMD) == 0)
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
