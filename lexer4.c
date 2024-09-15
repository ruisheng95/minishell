/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 21:49:14 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/16 00:14:50 by ethanlim         ###   ########.fr       */
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
	if (n == PIPE || n == REDIR_INPUT
		|| n == REDIR_OUT_APPEND || n == REDIR_OUT_OVERWRITE
		|| n == HEREDOC)
		return (1);
	return (0);
}

int	check_valid_list_helper_cuz_norminette(t_tokens *first_node,
	t_tokens *last_node)
{
	if (first_node->type == PIPE || check_operator(last_node->type) == 1)
	{
		if (check_operator(last_node->type) == 1)
			printf("syntax error near unexpected token '%s'\n",
				last_node->token);
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
			&& check_operator(temp->next->type) == 1)
		{
			ft_printf("syntax error near unexpected token '%s'\n",
				temp->token);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}
