/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 00:08:07 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/14 18:17:49 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

void	print_2d_array(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		printf("token %d = %s\n", i, cmd[i]);
		i++;
	}
}

int	exit_process(char **arg)
{
	int	exit_code;

	if (arg[1])
		exit_code = ft_atoi(arg[1]);
	else
		exit_code = 0;
	write(1, "exit\n", 5);
	exit(exit_code);
	return (0);
}

int	set_terminos_echo(int enable)
{
	struct termios	terminos_p;

	if (tcgetattr(STDOUT_FILENO, &terminos_p) == -1)
		return (-1);
	if (enable)
		terminos_p.c_lflag |= ECHOCTL;
	else
		terminos_p.c_lflag &= ~(ECHOCTL);
	if (tcsetattr(STDOUT_FILENO, TCSANOW, &terminos_p) == -1)
		return (-1);
	return (0);
}

int	waitpid_and_get_exit_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (0);
}
