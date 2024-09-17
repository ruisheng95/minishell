/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rng <rng@student.42kl.edu.my>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 00:37:25 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/17 22:57:29 by rng              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prepare_fd(t_cmd_list *templist, t_data *data);

void	exit_error(int n)
{
	if (n == 1)
		write(2, "Error: wrong pipex usage\n", 25);
	else
		perror("Error");
	exit(1);
}

void	exit_error_str(char *str, int n)
{
	int	len;

	len = ft_strlen(str);
	if (n == 1)
		write(2, "Error: command not found: ", 26);
	else
		write(2, "Error: no such file or directory: ", 35);
	write(2, str, len);
	write(2, "\n", 1);
	if (n == 1)
		exit(127);
	exit(1);
}

void	prepare_fd_and_signal(t_cmd_list *templist, t_data *data)
{
	prepare_fd(templist, data);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	set_terminos_echo(1);
}

void	execve_error(void)
{
	perror("execve: ");
	exit (126);
}
