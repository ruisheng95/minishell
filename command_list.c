/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rng <rng@student.42kl.edu.my>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 18:33:18 by rng               #+#    #+#             */
/*   Updated: 2024/09/15 22:14:59 by rng              ###   ########.fr       */
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

void	make_command_list_heredoc(t_cmd_list **head, t_cmd_list **lastnode,
	t_cmd_list **newnode, char *limiter)
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

t_cmd_list	*create_new_node(void)
{
	t_cmd_list	*newnode;

	newnode = malloc(sizeof(t_cmd_list));
	newnode->cmd = 0;
	newnode->pid = 0;
	newnode->in_fd = 0;
	newnode->out_fd = 1;
	newnode->next = NULL;
	newnode->prev = NULL;
	return (newnode);
}

void	set_prev_fd(t_cmd_list *newnode, t_node *templist, t_cmd_list **head,
	t_cmd_list **lastnode)
{
	if (templist->prev)
	{
		if (templist->prev->type == REDIR_INPUT)
		{
			newnode->in_fd = templist->prev->redir_in.fd;
		}
		else if (templist->prev->type == PIPE)
		{
			newnode->in_fd = templist->prev->piping.pipe_fd[0];
		}
		else if (templist->prev->type == REDIR_OUT_APPEND
			|| templist->prev->type == REDIR_OUT_OVERWRITE)
		{
			newnode->out_fd = templist->prev->redir_out.fd;
		}
		else if (templist->prev && templist->prev->type == HEREDOC)
			make_command_list_heredoc(head, lastnode, &newnode,
				templist->prev->heredoc_obj.limiter);
	}
}

t_cmd_list	*make_command_list(t_node *list)
{
	t_cmd_list	*newnode;
	t_cmd_list	*head;
	t_cmd_list	*lastnode;
	t_node		*templist;

	head = NULL;
	lastnode = NULL;
	templist = list;
	while (templist)
	{
		newnode = create_new_node();
		while (templist && templist->type != S_COMMAND)
			templist = templist->next;
		if (templist)
		{
			newnode->cmd = templist->simple_cmd.array;
			// newnode->in_fd = 0;
			// newnode->out_fd = 1;
			// if(templist->prev && templist->prev->type == redir_input)
			// 	newnode->in_fd = templist->prev->redir_in.fd;
			// if(templist->prev && templist->prev->type == Pipe)
			// 	newnode->in_fd = templist->prev->piping.pipe_fd[0];
			// if(templist->prev && (templist->prev->type == redir_out_append
			//	|| templist->prev->type == redir_out_overwrite))
			// 	newnode->out_fd = templist->prev->redir_out.fd;
			// if (templist->prev
			// && (templist->prev->type == redir_input
			// || templist->prev->type == redir_out_append
			// || templist->prev->type == redir_out_overwrite
			// || templist->prev->type == Pipe
			// || templist->prev->type == heredoc))
			set_prev_fd(newnode, templist, &head, &lastnode);
			// if(templist->prev && templist->prev->type == heredoc)
			// 	make_command_list_heredoc(&head, &lastnode, &newnode,
			//	templist->prev->heredoc_obj.limiter);
			while (templist->next
				&& (templist->next->type == PIPE
					|| templist->next-> type == REDIR_OUT_APPEND
					|| templist->next->type == REDIR_OUT_OVERWRITE
					|| templist->next->type == REDIR_INPUT
					|| templist->next->type == HEREDOC))
			{
				if (templist->next->type == PIPE)
					newnode->out_fd = templist->next->piping.pipe_fd[1];
				if (templist->next->type == REDIR_OUT_OVERWRITE
					|| templist->next->type == REDIR_OUT_APPEND)
					newnode->out_fd = templist->next->redir_out.fd;
				if (templist->next->type == REDIR_INPUT)
					newnode->in_fd = templist->next->redir_in.fd;
				if (templist->next->type == HEREDOC)
					make_command_list_heredoc(&head, &lastnode, &newnode,
						templist->next->heredoc_obj.limiter);
				templist = templist->next;
			}
		}
		if (!head)
			head = newnode;
		newnode->next = NULL;
		newnode->prev = NULL;
		if (lastnode)
		{
			lastnode->next = newnode;
			newnode->prev = lastnode;
		}
		lastnode = newnode;
		if (templist)
			templist = templist->next;
	}
	return (head);
}

void	print_cmd_list(t_cmd_list *list)
{
	t_cmd_list	*templist;
	int			i;

	templist = list;
	while (templist && templist->cmd)
	{
		i = 0;
		while (templist->cmd[i])
		{
			printf("|:%s:|", templist->cmd[i]);
			i++;
		}
		printf("|outfd = %d|", templist->out_fd);
		printf("|infd = %d|", templist->in_fd);
		printf("\n");
		templist = templist->next;
	}
}
// int	main(int argc, char **argv, char **envp)
// {
// 	t_tokens *list;
// 	t_node	*final_list;
// 	t_cmd_list *cmdlist;
// 	char *str = "echo hi > outfile1 | echo bye > outfile2";
// 	t_data data;
// 	data.envp = envp;
// 	if((data.tokens = lexer(str, &data)) == NULL)
// 		exit(1);
// 	data.tokens = lexer(str,&data);
// 	// print_token_array(data.tokens);
// 	printf("-----------------------------------\n");
// 	list = init_token_list(&data);
// 	identify_tokens_list(list);
// 	printf("-----------------------------------\n");
// 	expansion(list, &data);
// 	tokens_list_processing(list);
// 	identify_tokens_list2(list);
// 	remove_quotes_from_token_list(list);
// 	if(check_valid_list(list) == 1)
// 		exit(1);
// 	// print_tokens_list(list);
// 	printf("-----------------------------------\n");
// 	final_list = make_final_list(list); //<- this causes the seg fault
// 	printf("-----------------------------------\n");
// 	print_final_list(final_list);
// 	printf("-----------------------------------\n");
// 	printf("-----------------------------------\n");
// 	cmdlist = make_command_list(final_list);
// 	print_cmd_list(cmdlist);
// }
