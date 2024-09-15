/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 23:20:59 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/16 00:13:55 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipe_node(t_node *node)
{
	if (pipe(node->piping.pipe_fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
}

void	error_str(int n, char *str)
{
	int	len;

	len = ft_strlen(str);
	if (n == 1)
		write(2, "Error: command not found: ", 26);
	else
		write(2, "Error: no such file or directory: ", 35);
	write(2, str, len);
	write(2, "\n", 1);
}

int	handle_token_type(t_node *newnode, t_tokens **token)
{
	if ((*token)->type == PIPE)
		handle_pipe_token(newnode, token);
	else if ((*token)->type == REDIR_INPUT)
		return (handle_redir_input_token(newnode, token));
	else if ((*token)->type == REDIR_OUT_APPEND)
		return (handle_redir_out_append_token(newnode, token));
	else if ((*token)->type == REDIR_OUT_OVERWRITE)
		return (handle_redir_out_overwrite_token(newnode, token));
	else if ((*token)->type == HEREDOC)
		return (handle_heredoc_token(newnode, token));
	else if ((*token)->type == COMMAND)
		handle_command_token(newnode, token);
	else
		*token = (*token)->next;
	return (0);
}

t_node	*make_final_list(t_tokens *tokens)
{
	t_node		*list;
	t_node		*newnode;
	t_node		*last_node;
	t_tokens	*token;

	list = NULL;
	last_node = NULL;
	token = tokens;
	while (token)
	{
		newnode = create_node(token);
		if (!newnode)
			return (NULL);
		if (handle_token_type(newnode, &token) == -1)
			return (NULL);
		add_node_to_list(&list, &last_node, newnode);
	}
	return (list);
}

t_node	*make_final_list_heredoc(t_node *list)
{
	t_node	*head_node;
	t_node	*newnode;

	head_node = list;
	while (list)
	{
		if (list->type == HEREDOC)
		{
			if (!list->prev && !list->next)
			{
				newnode = malloc(sizeof(t_node));
				newnode->type = S_COMMAND;
				newnode->simple_cmd.cmd = ft_strdup("true");
				newnode->simple_cmd.array = malloc(sizeof(char *) * 2);
				newnode->simple_cmd.array[0] = ft_strdup("true");
				newnode->simple_cmd.array[1] = 0;
				list->next = newnode;
				newnode->prev = list;
				newnode->next = 0;
			}
			else if (!list->prev && list->next && list->next->type != S_COMMAND)
			{
				newnode = malloc(sizeof(t_node));
				newnode->type = S_COMMAND;
				newnode->simple_cmd.cmd = ft_strdup("true");
				newnode->simple_cmd.array = malloc(sizeof(char *) * 2);
				newnode->simple_cmd.array[0] = ft_strdup("true");
				newnode->simple_cmd.array[1] = 0;
				list->next->prev = newnode;
				newnode->next = list->next;
				newnode->prev = list;
				list->next = newnode;
			}
		}
		list = list->next;
	}
	return (head_node);
}

// void	print_final_list(t_node *list)
// {
// 	while(list)
// 	{
// 		printf("|%d|", list->type);
// 		if(list->type == redir_input)
// 		{
// 			printf("|infile: %s, fd : %d|", list->redir_in.in_file, list->redir_in.fd);
// 		}
// 		if(list->type == s_command)
// 		{
// 			printf("|%s|", list->simple_cmd.cmd);
// 			int i = 0;
// 			printf("|");
// 			while(list->simple_cmd.array[i])
// 			{
// 				printf(":%s:", list->simple_cmd.array[i]);
// 				i++;
// 			}
// 			// i = 0;
// 			printf("|");
// 		}
// 		if(list->type == redir_out_append || list->type == redir_out_overwrite)
// 		{
// 			printf("|outfile: %s, fd : %d|", list->redir_out.outfile, list->redir_out.fd);
// 		}
// 		if (list->type == Pipe)
// 		{
// 			printf("pipe fd[0] = %d, fd[1] = %d", list->piping.pipe_fd[0], list->piping.pipe_fd[1]);
// 		}
// 		if (list->type == heredoc)
// 		{
// 			printf("heredoc lim: %s", list->heredoc_obj.limiter);
// 		}
// 		list = list->next;
// 		printf("\n");
// 	}
// }
// int	main(int argc, char **argv, char **envp)
// {
// 	t_tokens *list;
// 	t_node	*final_list;
// 	char *str = "$INVALID";
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
// 	return 0;
// }
