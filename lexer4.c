/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 21:49:14 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/14 15:00:39 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokens	*get_last_node(t_tokens *tokens)
{
	t_tokens	*list;

	list = tokens;
	while (list->next)
	{
		list = list->next;
	}
	return (list);
}

int	check_operator(int n)
{
	if (n == Pipe || n == redir_input
		|| n == redir_out_append || n == redir_out_overwrite
		|| n == heredoc)
		return (1);
	return (0);
}

int	check_valid_list_helper_cuz_norminette(t_tokens *first_node,
	t_tokens *last_node)
{
	if (first_node->type == Pipe || check_operator(last_node->type) == 1)
	{
		if (check_operator(last_node->type) == 1)
			printf("syntax error near unexpected token 'newline'\n");
		else
			printf("syntax error near unexpected token '|'\n");
		return (1);
	}
	return (0);
}

int	check_valid_list(t_tokens *list)
{
	t_tokens	*last_node;
	t_tokens	*first_node;
	t_tokens	*temp;

	first_node = list;
	last_node = get_last_node(list);
	temp = list;
	if (check_valid_list_helper_cuz_norminette(first_node, last_node) == 1)
		return (1);
	while (temp->next)
	{
		if (check_operator(temp->type) == 1
			&& check_operator(temp->next->type) == 1 && temp->next == last_node)
		{
			ft_printf("syntax error near unexpected token 'newline'\n", temp->token);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}
