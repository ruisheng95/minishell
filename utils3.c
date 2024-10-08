/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 01:18:55 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/21 17:56:29 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run(char *line, t_data *data);

int	change_exit_code(t_data *data, int code)
{
	data->exit_code = code;
	return (1);
}

void	init_data_struct(t_data *data, char **envp)
{
	data->envp = malloc(sizeof(char *) * (total_strings(envp) + 1));
	malloc_dup_env(data->envp, envp);
	increment_shell_lvl(data);
	data->saved_in_fd = dup(0);
	data->saved_out_fd = dup(1);
	data->exit_code = 0;
	data->instr_list = 0;
}

void	waitpid_and_check_exit_status(int *exit_status, t_cmd_list	*templist)
{
	while (templist)
	{
		*exit_status = waitpid_and_get_exit_status(templist->pid);
		if (WIFSIGNALED(*exit_status) && WTERMSIG(*exit_status) == SIGQUIT)
		{
			ft_printf("Quit\n");
			break ;
		}
		else if (WIFSIGNALED(*exit_status) && WTERMSIG(*exit_status) == SIGINT)
		{
			ft_printf("\n");
			break ;
		}
		templist = templist->next;
	}
}

int	do_minishell_because_norm(t_data *data)
{
	char	*line;

	line = readline("Minishell$");
	if (line == NULL)
	{
		write(1, "exit\n", 5);
		return (-1);
	}
	if (line && line[0])
	{
		add_history(line);
		run(line, data);
	}
	free(line);
	return (0);
}

void	write_error_msg(char *str1, char *str2, char *str3)
{
	if (str1)
		write(2, str1, ft_strlen(str1));
	if (str2)
		write(2, str2, ft_strlen(str2));
	if (str3)
		write(2, str3, ft_strlen(str3));
	return ;
}
