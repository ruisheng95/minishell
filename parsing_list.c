/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rng <rng@student.42kl.edu.my>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 18:38:46 by rng               #+#    #+#             */
/*   Updated: 2024/09/15 20:35:37 by rng              ###   ########.fr       */
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
// void	init_parsing_list(int type, t_node *node, t_tokens *token)
// {
// 	if(type == Pipe)
// 	{
// 		node->type = Pipe;
// 		init_pipe_node(node);
// 	}
// 	else if(type = command)
// 	{
// 		node->type = simple_command;
// 		node->simple_cmd.cmd = token->token;
// 	}
// 	else if(type == redir_input)
// 	{

// 	}
// }

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

t_node	*make_final_list(t_tokens *tokens)
{
	t_node		*list;
	t_node		*newnode;
	t_node		*last_node;
	t_tokens	*token;
	int			i;

	list = NULL;
	newnode = NULL;
	last_node = NULL;
	token = tokens;
	i = 0;
	while (token)
	{
		newnode = malloc(sizeof(t_node));
		newnode->type = token->type;
		if (token->type == PIPE)
		{
			init_pipe_node(newnode);
			token = token->next;
		}
		else if (token->type == REDIR_INPUT)
		{
			newnode->type = REDIR_INPUT;
			if (token->next->type == INFILE)
			{
				newnode->redir_in.in_file = token->next->token;
				newnode->redir_in.fd = open(newnode->redir_in.in_file,
						O_RDONLY);
				if (newnode->redir_in.fd == -1)
				{
					error_str(2, newnode->redir_in.in_file);
					return (NULL);
				}
				token = token->next;
				token = token->next;
			}
			else
			{
				free(newnode);
				return (NULL);
			}
		}
		else if (token->type == REDIR_OUT_APPEND)
		{
			newnode->type = REDIR_OUT_APPEND;
			if (token->next->type == OUTFILE_APPEND)
			{
				newnode->redir_out.outfile = token->next->token;
				newnode->redir_out.fd = open(newnode->redir_out.outfile,
						O_WRONLY | O_CREAT | O_APPEND, 0777);
				token = token->next;
				token = token->next;
			}
			else
			{
				free(newnode);
				return (NULL);
			}
		}
		else if (token->type == REDIR_OUT_OVERWRITE)
		{
			newnode->type = REDIR_OUT_OVERWRITE;
			if (token->next->type == OUTFILE_OVERWRITE)
			{
				newnode->redir_out.outfile = token->next->token;
				newnode->redir_out.fd = open(newnode->redir_out.outfile,
						O_WRONLY | O_TRUNC | O_CREAT, 0777);
				token = token->next;
				token = token->next;
			}
			else
			{
				free(newnode);
				return (NULL);
			}
		}
		else if (token->type == HEREDOC)
		{
			newnode->type = HEREDOC;
			if (token->next->type == HEREDOC_LIM)
			{
				newnode->heredoc_obj.limiter = token->next->token;
				token = token->next;
				token = token->next;
			}
			else
			{
				free(newnode);
				return (NULL);
			}
		}
		else if (token->type == COMMAND)
		{
			newnode->type = S_COMMAND;
			if (token->token != NULL && token->token[0] != '\0')
			{
				newnode->simple_cmd.cmd = ft_strdup(token->token);
				newnode->simple_cmd.array = ft_calloc(sizeof(char *), 1000);
				newnode->simple_cmd.array[i] = ft_strdup(token->token);
				i++;
				token = token->next;
				while (token != NULL && token->type == ARGUMENTS)
				{
					newnode->simple_cmd.array[i] = ft_strdup(token->token);
					i++;
					token = token->next;
				}
				newnode->simple_cmd.array[i] = NULL;
				i = 0;
			}
			else
			{
				newnode->simple_cmd.cmd = ft_strdup("");
				newnode->simple_cmd.array = malloc(sizeof(char *) * 2);
				newnode->simple_cmd.array[0] = ft_strdup("");
				newnode->simple_cmd.array[1] = NULL;
				token = token->next;
			}
		}
		else
			token = token->next;
		if (last_node)
		{
			last_node->next = newnode;
			newnode->prev = last_node;
		}
		else
			newnode->prev = NULL;
		last_node = newnode;
		newnode->next = NULL;
		if (!list)
			list = newnode;
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

void	print_final_list(t_node *list)
{
	int	i;

	while (list)
	{
		printf("|%d|", list->type);
		if (list->type == REDIR_INPUT)
		{
			printf("|infile: %s, fd : %d|", list->redir_in.in_file,
				list->redir_in.fd);
		}
		if (list->type == S_COMMAND)
		{
			printf("|%s|", list->simple_cmd.cmd);
			i = 0;
			printf("|");
			while (list->simple_cmd.array[i])
			{
				printf(":%s:", list->simple_cmd.array[i]);
				i++;
			}
			printf("|");
		}
		if (list->type == REDIR_OUT_APPEND || list->type == REDIR_OUT_OVERWRITE)
		{
			printf("|outfile: %s, fd : %d|", list->redir_out.outfile,
				list->redir_out.fd);
		}
		if (list->type == PIPE)
		{
			printf("pipe fd[0] = %d, fd[1] = %d", list->piping.pipe_fd[0],
				list->piping.pipe_fd[1]);
		}
		if (list->type == HEREDOC)
		{
			printf("heredoc lim: %s", list->heredoc_obj.limiter);
		}
		list = list->next;
		printf("\n");
	}
}
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
