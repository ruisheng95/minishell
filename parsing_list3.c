/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_list3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 23:32:39 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/15 23:44:12 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_token(t_node *newnode, t_tokens **token)
{
	init_pipe_node(newnode);
	*token = (*token)->next;
}

t_node	*create_node(t_tokens *token)
{
	t_node	*newnode;

	newnode = malloc(sizeof(t_node));
	if (!newnode)
		return (NULL);
	newnode->type = token->type;
	return (newnode);
}

void	add_node_to_list(t_node **list, t_node **last_node, t_node *newnode)
{
	if (*last_node)
	{
		(*last_node)->next = newnode;
		newnode->prev = *last_node;
	}
	else
	{
		newnode->prev = NULL;
	}
	*last_node = newnode;
	newnode->next = NULL;
	if (!*list)
	{
		*list = newnode;
	}
}
