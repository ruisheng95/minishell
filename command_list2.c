/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_list2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 18:11:48 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/16 00:10:27 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_heredoc_cmd(char *lim)
{
	char	**result;

	result = malloc(sizeof(*result) * 3);
	result[0] = ft_strdup(HEREDOC_CMD);
	result[1] = lim;
	result[2] = 0;
	return (result);
}

void	make_command_list_heredoc(t_cmd_list **head,
	t_cmd_list **lastnode, t_cmd_list **newnode, char *limiter)
{
	t_cmd_list	*heredoc_node;
	int			pipe_fd[2];

	heredoc_node = malloc(sizeof(t_cmd_list));
	heredoc_node->cmd = get_heredoc_cmd(limiter);
	heredoc_node->pid = 0;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	heredoc_node->in_fd = 0;
	heredoc_node->out_fd = pipe_fd[1];
	(*newnode)->in_fd = pipe_fd[0];
	if (*lastnode)
	{
		(*lastnode)->next = heredoc_node;
		heredoc_node->prev = *lastnode;
	}
	if (!*head)
		*head = heredoc_node;
	*lastnode = heredoc_node;
}
