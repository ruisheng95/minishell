/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rng <rng@student.42kl.edu.my>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 20:37:04 by rng               #+#    #+#             */
/*   Updated: 2024/09/20 22:48:18 by rng              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cmd_list(t_cmd_list *list);
void	close_non_std_fd(int ignore_fd_1, int ignore_fd_2);
void	waitpid_and_check_exit_status(int *exit_status, t_cmd_list	*templist);
int		do_minishell_because_norm(t_data *data);
// void	print_cmd_list(t_cmd_list *list);

void	signal_handler(int sig)
{
	(void)sig;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	the_real_actual_main(t_data *data)
{
	int			exit_status;
	t_cmd_list	*cmdlist;
	t_cmd_list	*templist;
	int			n;

	exit_status = 0;
	cmdlist = make_command_list(data->instr_list);
	templist = cmdlist;
	while (templist && templist->cmd)
	{
		n = execute(templist->cmd, &data, templist);
		if (n != 0)
			return (n);
		templist = templist->next;
	}
	templist = cmdlist;
	close_non_std_fd(data->saved_in_fd, data->saved_out_fd);
	waitpid_and_check_exit_status(&exit_status, templist);
	free_t_cmd_list(cmdlist);
	return (exit_status);
}

t_tokens	*process_tokens(t_data *data)
{
	t_tokens	*list;

	list = init_token_list(data);
	if (list == NULL)
		return (NULL);
	identify_tokens_list(list);
	expansion(list, data);
	tokens_list_processing(list);
	remove_quotes_from_token_list(list);
	identify_tokens_list2(list);
	return (list);
}

int	run(char *line, t_data *data)
{
	t_tokens	*list;

	data->input_line = line;
	data->tokens = lexer(line, data);
	signal(SIGINT, SIG_IGN);
	if (data->tokens == NULL)
		return (change_exit_code(data, 0));
	list = process_tokens(data);
	if (!list)
		return (1);
	if (check_valid_list(list) == 1)
		return (cleanup(data, list), change_exit_code(data, 1));
	data->instr_list = make_final_list(list);
	if (data->instr_list == NULL)
		return (cleanup(data, list), change_exit_code(data, 1));
	data->instr_list = make_final_list_heredoc(data->instr_list);
	if (data->instr_list != NULL)
		data->exit_code = the_real_actual_main(data);
	cleanup(data, list);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)(argc);
	(void)(argv);
	init_data_struct(&data, envp);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		signal(SIGINT, signal_handler);
		set_terminos_echo(0);
		if (do_minishell_because_norm(&data) == -1)
			break ;
	}
	set_terminos_echo(1);
	free_2d_array(data.envp);
	return (0);
}
