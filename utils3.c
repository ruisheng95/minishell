/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rng <rng@student.42kl.edu.my>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 01:18:55 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/18 00:10:28 by rng              ###   ########.fr       */
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
