/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 17:53:53 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/16 00:09:55 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_list	*create_new_node(void)
{
	t_cmd_list	*newnode;

	newnode = malloc(sizeof(t_cmd_list));
	if (!newnode)
		return (NULL);
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

void	process_next_nodes(t_cmd_list *newnode,
	t_node **templist, t_cmd_list **head, t_cmd_list **lastnode)
{
	while ((*templist)->next
		&& ((*templist)->next->type == PIPE
			|| (*templist)->next->type == REDIR_OUT_APPEND
			|| (*templist)->next->type == REDIR_OUT_OVERWRITE
			|| (*templist)->next->type == REDIR_INPUT
			|| (*templist)->next->type == HEREDOC))
	{
		if ((*templist)->next->type == PIPE)
			newnode->out_fd = (*templist)->next->piping.pipe_fd[1];
		if ((*templist)->next->type == REDIR_OUT_OVERWRITE
			|| (*templist)->next->type == REDIR_OUT_APPEND)
			newnode->out_fd = (*templist)->next->redir_out.fd;
		if ((*templist)->next->type == REDIR_INPUT)
			newnode->in_fd = (*templist)->next->redir_in.fd;
		if ((*templist)->next->type == HEREDOC)
			make_command_list_heredoc(head, lastnode, &newnode,
				(*templist)->next->heredoc_obj.limiter);
		*templist = (*templist)->next;
	}
}

void	link_nodes(t_cmd_list **head,
	t_cmd_list **lastnode, t_cmd_list *newnode, t_node **templist)
{
	if (!*head)
		*head = newnode;
	if (*lastnode)
	{
		(*lastnode)->next = newnode;
		newnode->prev = *lastnode;
	}
	*lastnode = newnode;
	if (*templist)
		*templist = (*templist)->next;
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
			set_prev_fd(newnode, templist, &head, &lastnode);
			process_next_nodes(newnode, &templist, &head, &lastnode);
		}
		link_nodes(&head, &lastnode, newnode, &templist);
	}
	return (head);
}

// void	print_cmd_list(t_cmd_list *list)
// {
// 	t_cmd_list	*templist = list;
// 	while(templist && templist->cmd)
// 	{
// 		int i = 0;
// 		while(templist->cmd[i])
// 		{
// 			printf("|:%s:|", templist->cmd[i]);
// 			i++;
// 		}
// 		printf("|outfd = %d|", templist->out_fd);
// 		printf("|infd = %d|", templist->in_fd);
// 		printf("\n");
// 		templist = templist->next;
// 	}
// }
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
