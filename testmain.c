/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testmain.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 22:33:03 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/21 23:01:50 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"

// void	print_final_list(t_node *list)
// {
// 	while(list)
// 	{
// 		printf("|%d|", list->type);
// 		if(list->type == REDIR_INPUT)
// 		{
// 			printf("|infile: %s, fd : %d|", list->redir_in.in_file, 
// 				list->redir_in.fd);
// 		}
// 		if(list->type == S_COMMAND)
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
// 		if(list->type == REDIR_OUT_APPEND || list->type == REDIR_OUT_OVERWRITE)
// 		{
// 			printf("|outfile: %s, fd : %d|", list->redir_out.outfile,
// 				list->redir_out.fd);
// 		}
// 		if (list->type == PIPE)
// 		{
// 			printf("pipe fd[0] = %d, fd[1] = %d", list->piping.pipe_fd[0],
// 				list->piping.pipe_fd[1]);
// 		}
// 		if (list->type == HEREDOC)
// 		{
// 			printf("heredoc lim: %s", list->heredoc_obj.limiter);
// 		}
// 		list = list->next;
// 		printf("\n");
// 	}
// }

// void	print_token_array(char **str)
// {
// 	int	i;

// 	i = 0;
// 	if (!str)
// 	{
// 		printf("ERORRR LOLLL\n");
// 		return ;
// 	}
// 	while (str[i])
// 	{
// 		printf("token %d = %s\n", i, str[i]);
// 		i++;
// 	}
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	t_tokens *list;
// 	t_node	*final_list;
// 	t_cmd_list *cmdlist;
// 	char *str = "cat < pwd.c | sort > outfile";
// 	t_data data;
// 	data.envp = envp;
// 	if((data.tokens = lexer(str, &data)) == NULL)
// 		exit(1);
// 	data.tokens = lexer(str,&data);
// 	printf("-----------------------------------\n");
// 	printf("-------- tokens splitting ---------\n");
// 	printf("-----------------------------------\n");
// 	print_token_array(data.tokens);
// 	list = init_token_list(&data);
// 	identify_tokens_list(list);
// 	printf("-----------------------------------\n");
// 	printf("---- identifying special chars ----\n");
// 	printf("-----------------------------------\n");
// 	print_tokens_list(list);
// 	expansion(list, &data);
// 	printf("-----------------------------------\n");
// 	printf("------------ expansion ------------\n");
// 	printf("-----------------------------------\n");
// 	print_tokens_list(list);
// 	tokens_list_processing(list);
// 	identify_tokens_list2(list);
// 	remove_quotes_from_token_list(list);
// 	if(check_valid_list(list) == 1)
// 		exit(1);
// 	printf("-----------------------------------\n");
// 	printf("------- classifying strings -------\n");
// 	printf("-----------------------------------\n");
// 	print_tokens_list(list);
// 	printf("-----------------------------------\n");
// 	printf("------- grouping tokens tgt -------\n");
// 	printf("-----------------------------------\n");
// 	final_list = make_final_list(list);
// 	print_final_list(final_list);
// 	printf("-----------------------------------\n");
// 	printf("--- make the execution sequence ---\n");
// 	printf("-----------------------------------\n");
// 	cmdlist = make_command_list(final_list);
// 	print_cmd_list(cmdlist);
// 	printf("-----------------------------------\n");
// }
