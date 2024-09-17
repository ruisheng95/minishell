/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_list4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rng <rng@student.42kl.edu.my>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 23:06:21 by rng               #+#    #+#             */
/*   Updated: 2024/09/17 23:09:06 by rng              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*create_node_heredoc(void);

void	standalone_heredoc(t_node *list)
{
	t_node	*newnode;

	newnode = create_node_heredoc();
	list->next = newnode;
	newnode->prev = list;
	newnode->next = 0;
}

void	non_standalone_heredoc(t_node *list)
{
	t_node	*newnode;

	newnode = create_node_heredoc();
	list->next->prev = newnode;
	newnode->next = list->next;
	newnode->prev = list;
	list->next = newnode;
}
