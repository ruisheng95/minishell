#include "minishell.h"

char	**get_heredoc_cmd(char *lim)
{
	char	**result;

	result = malloc(sizeof(*result) * 3);
	result[0] = ft_strdup(heredoc_cmd);
	result[1] = lim;
	result[2] = 0;
	return (result);
}

void	make_command_list_heredoc(t_cmd_list **head, t_cmd_list **lastnode, t_cmd_list **newnode, char *limiter)
{
	t_cmd_list	*heredoc_node;
	int 		pipe_fd[2];

	heredoc_node = malloc(sizeof(t_cmd_list));
	heredoc_node->cmd = get_heredoc_cmd(limiter);
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

t_cmd_list *make_command_list(t_node *list)
{
	t_cmd_list *newnode;
	t_cmd_list *head = NULL;
	t_cmd_list *lastnode = NULL;
	t_node *templist = list;
	while(templist)
	{
		newnode = malloc(sizeof(t_cmd_list));
		while(templist->type != s_command && templist)
			templist = templist->next;
		if(templist)
		{
			newnode->cmd = templist->simple_cmd.array;
			newnode->in_fd = 0;
			newnode->out_fd = 1;
			if(templist->prev && templist->prev->type == redir_input)
				newnode->in_fd = templist->prev->redir_in.fd;
			if(templist->prev && templist->prev->type == Pipe)
				newnode->in_fd = templist->prev->piping.pipe_fd[0];
			if(templist->prev && (templist->prev->type == redir_out_append || templist->prev->type == redir_out_overwrite))
				newnode->out_fd = templist->prev->redir_out.fd;
			if(templist->prev && templist->prev->type == heredoc)
				make_command_list_heredoc(&head, &lastnode, &newnode, templist->prev->heredoc_obj.limiter);
			while(templist->next && 
				(templist->next->type == Pipe || 
				templist->next-> type == redir_out_append ||
				templist->next->type == redir_out_overwrite ||
				templist->next->type == redir_input ||
				templist->next->type == heredoc))
				{
					if(templist->next->type == Pipe)
						newnode->out_fd = templist->next->piping.pipe_fd[1];
					if(templist->next->type == redir_out_overwrite || templist->next->type == redir_out_append)
						newnode->out_fd = templist->next->redir_out.fd;
					if(templist->next->type == redir_input)
						newnode->in_fd = templist->next->redir_in.fd;
					if(templist->next->type == heredoc)
						make_command_list_heredoc(&head, &lastnode, &newnode, templist->next->heredoc_obj.limiter);
					templist = templist->next;
				}
		}
		if(!head)
			head = newnode;
		newnode->next = NULL;
		newnode->prev = NULL;
		if(lastnode)
		{
			lastnode->next = newnode;
			newnode->prev = lastnode;
		}
		lastnode = newnode;
		templist = templist->next;
	}
	return head;
}

void	print_cmd_list(t_cmd_list *list)
{
	t_cmd_list *templist = list;
	while(templist)
	{
		int i = 0;
		while(templist->cmd[i])
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
// 	char *str = "cat < infile | grep 'a'";
// 	t_data data;
// 	data.tokens = lexer(str,envp);
// 	// print_token_array(data.tokens);
// 	printf("-----------------------------------\n");
// 	list = init_token_list(&data);
// 	identify_tokens_list(list);
// 	identify_tokens_list2(list);
// 	if(check_valid_list(list) == 1)
// 		exit(1);
// 	// print_tokens_list(list);
// 	final_list = make_final_list(list);
// 	print_final_list(final_list);
// 	printf("-----------------------------------\n");
// 	cmdlist = make_command_list(final_list);
// 	print_cmd_list(cmdlist);
// }

