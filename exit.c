/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 02:11:44 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/15 02:17:23 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	check_alpha(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '+' && str[i] != '-' && (str[i] < '0' || str[i] > '9'))
			return (1);
		i++;
	}
	return (0);
}

int	exit_process(char **arg, t_data *data)
{
	int	exit_code;

	if(arg[2] != NULL)
	{
		printf("Error: exit: too many arguments\n");
		return (1);
	}
	if (arg[1])
	{
		if (check_alpha(arg[1]) == 0)
			exit_code = ft_atoi(arg[1]);
		else
		{
			printf("exit: %s: numeric argument required\n", arg[1]);
			exit_code = 255;
		}
	}
	else
		exit_code = data->exit_code;
	write(1, "exit\n", 5);
	exit(exit_code);
	return (0);
}
