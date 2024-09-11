#include "minishell.h"

void	print_cmd_list(t_cmd_list *list);

void	signal_handler(int sig)
{
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

char	*get_readline_prompt(char **env)
{
	char	*temp1;
	char	**temp_split;
	char	temp_buffer[1024];
	char	*temp2;
	char	*res;
	int		i;

	i = 0;
	temp1 = ft_strjoin(ft_strjoin("\033[34m", ft_strjoin(get_expand_string("USER", env), "@Minishell$ ")), "\033[0m");
	if (getcwd(temp_buffer,sizeof(temp_buffer)) == NULL)
	{
		perror("getcwd error");
		exit(1);
	}
	temp_split = ft_split(temp_buffer, '/');
	while(temp_split[i + 1] != NULL)
		i++;
	temp2 = ft_strjoin(temp1, temp_split[i]);
	res = ft_strjoin(temp2, ":");
	free(temp1);
	free(temp2);
	free_2d_array(temp_split);
	return res;
}

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

int the_real_actual_main(t_data *data)
{
	int exit_status = 0;

	t_cmd_list *cmdlist = make_command_list(data->instr_list);
	t_cmd_list *templist = cmdlist;
	// print_cmd_list(templist);
	while(templist && templist->cmd)
	{
		if(execute(templist->cmd, &data, templist) != 0)
			return 1;
		templist = templist->next;
	}
	templist = cmdlist;
	for (int i = 3; i < 1000; i++)
	{
		if(i != data->saved_in_fd && i != data->saved_out_fd)
			close(i);
	}
	while (templist)
	{
		waitpid(templist->pid, &exit_status, 0);
		if (WIFSIGNALED(exit_status) && WTERMSIG(exit_status) == SIGQUIT)
		{
			ft_printf("Quit\n");
			break;
		}
		else if (WIFSIGNALED(exit_status) && WTERMSIG(exit_status) == SIGINT)
		{
			ft_printf("\n");
			break;
		}
		templist = templist->next;
	}
	free_t_cmd_list(cmdlist);
	return 0;
}
int	run(char *line, t_data *data)
{
	t_tokens	*list;
	pid_t	pid;

	data->input_line = line;
	data->tokens = lexer(line, data);
	signal(SIGINT, SIG_IGN);
	// process_tokens(&data);
	if (data->tokens == NULL)
	{
		// free(data->envp);
		return 1;
	}
	list = init_token_list(data);
	identify_tokens_list(list);
	expansion(list, data);
	tokens_list_processing(list);
	remove_quotes_from_token_list(list);
	identify_tokens_list2(list);
	// print_tokens_list(list);
	data->exit_code = check_valid_list(list);
	if(data->exit_code == 1)
		return(1);
	// printf("----------------------");
	data->instr_list = make_final_list(list);
	data->instr_list = make_final_list_heredoc(data->instr_list);
	// execute(data.tokens, data.envp);
	// print_final_list(data->instr_list);
	if(data->instr_list != NULL)
	{
		data->exit_code = the_real_actual_main(data);
	}
	free_t_tokens(list);
	free_2d_array(data->tokens);
	free_t_node(data->instr_list);
	return 0;
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

int main(int argc, char **argv, char **envp) 
{
	pid_t	pid;
	t_data	data;
	char	*line;
	// char	*readline_prompt;

	init_data_struct(&data, envp);
	signal(SIGQUIT, SIG_IGN);
    while (1)
	{
		signal(SIGINT, signal_handler);
		// readline_prompt = get_readline_prompt(envp);
		set_terminos_echo(0);
    	line = readline("Minishell$");
		if (line == NULL)
		{
			write(1,"exit\n",5);
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
    return 0;
}