/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 20:37:04 by rng               #+#    #+#             */
/*   Updated: 2024/09/16 00:14:40 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cmd_list(t_cmd_list *list);
void	close_non_std_fd(int ignore_fd_1, int ignore_fd_2);

void	signal_handler(int sig)
{
	(void)sig;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

// char	*get_readline_prompt(char **env)
// {
// 	char	*temp1;
// 	char	**temp_split;
// 	char	temp_buffer[1024];
// 	char	*temp2;
// 	char	*res;
// 	int		i;

// 	i = 0;
// 	temp1 = ft_strjoin(ft_strjoin("\033[34m", ft_strjoin(get_expand_string("USER", env), "@Minishell$ ")), "\033[0m");
// 	if (getcwd(temp_buffer,sizeof(temp_buffer)) == NULL)
// 	{
// 		perror("getcwd error");
// 		exit(1);
// 	}
// 	temp_split = ft_split(temp_buffer, '/');
// 	while(temp_split[i + 1] != NULL)
// 		i++;
// 	temp2 = ft_strjoin(temp1, temp_split[i]);
// 	res = ft_strjoin(temp2, ":");
// 	free(temp1);
// 	free(temp2);
// 	free_2d_array(temp_split);
// 	return res;
// }

// t_tokens*	process_tokens(t_data *data)
// {
// 	t_tokens *list;
// 	t_tokens *newnode;
// 	int i;
// 	i = 0;
// 	while(data->tokens[i])
// 	{
// 		newnode = malloc(sizeof(t_tokens *));
// 		newnode->token = data->tokens[i];
// 		if(!list)
// 		{
// 			list = newnode;
// 			newnode->next = NULL;
// 			newnode->prev = NULL;
// 		}
// 		else
// 		{
// 			list->next = newnode;
// 			newnode->prev = list;
// 			newnode->next = NULL;
// 		}
// 		i++;
// 	}
// 	return list;
// }

int	the_real_actual_main(t_data *data)
{
	int			exit_status;
	t_cmd_list	*cmdlist;
	t_cmd_list	*templist;
	int			n;

	exit_status = 0;
	cmdlist = make_command_list(data->instr_list);
	templist = cmdlist;
	// print_cmd_list(templist);
	while (templist && templist->cmd)
	{
		n = execute(templist->cmd, &data, templist);
		if (n != 0)
			return (n);
		templist = templist->next;
	}
	templist = cmdlist;
	close_non_std_fd(data->saved_in_fd, data->saved_out_fd);
	while (templist)
	{
		exit_status = waitpid_and_get_exit_status(templist->pid);
		if (WIFSIGNALED(exit_status) && WTERMSIG(exit_status) == SIGQUIT)
		{
			ft_printf("Quit\n");
			break ;
		}
		else if (WIFSIGNALED(exit_status) && WTERMSIG(exit_status) == SIGINT)
		{
			ft_printf("\n");
			break ;
		}
		templist = templist->next;
	}
	free_t_cmd_list(cmdlist);
	return (exit_status);
}

int	run(char *line, t_data *data)
{
	t_tokens	*list;
	int			error;

	data->input_line = line;
	data->tokens = lexer(line, data);
	signal(SIGINT, SIG_IGN);
	// process_tokens(&data);
	if (data->tokens == NULL)
	{
		data->exit_code = 1;
		return (1);
	}
	if ((list = init_token_list(data)) == NULL)
		return (0);
	identify_tokens_list(list);
	expansion(list, data);
	tokens_list_processing(list);
	remove_quotes_from_token_list(list);
	identify_tokens_list2(list);
	// print_tokens_list(list);
	error = check_valid_list(list);
	if (error == 1)
	{
		data->exit_code = 1;
		return (1);
	}
	data->instr_list = make_final_list(list);
	if (data->instr_list == NULL)
	{
		data->exit_code = 1;
		return (1);
	}
	data->instr_list = make_final_list_heredoc(data->instr_list);
	// print_final_list(data->instr_list);
	if (data->instr_list != NULL)
	{
		data->exit_code = the_real_actual_main(data);
	}
	// printf("exit code = %d\n", data->exit_code);
	free_t_tokens(list);
	free_2d_array(data->tokens);
	free_t_node(data->instr_list);
	return (0);
}

void	init_data_struct(t_data *data, char **envp)
{
	data->envp = malloc(sizeof(char *) * (total_strings(envp) + 1));
	malloc_dup_env(data->envp, envp);
	increment_shell_lvl(data);
	data->saved_in_fd = dup(0);
	data->saved_out_fd = dup(1);
	data->exit_code = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	char	*line;

	(void)(argc);
	(void)(argv);
	init_data_struct(&data, envp);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		signal(SIGINT, signal_handler);
		set_terminos_echo(0);
		line = readline("Minishell$");
		if (line == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (line && line[0])
		{
			add_history(line);
			run(line, &data);
		}
		free(line);
	}
	set_terminos_echo(1);
	free_2d_array(data.envp);
	return (0);
}
