/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 00:42:37 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/13 00:43:35 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_flag(char *str)
{
	int	i;

	i = 2;
	if (str[0] == '-' && str[1] == 'n')
	{
		while (str[i] && str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (1);
	}
	return (0);
}

int	echo(char **cmd)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if (!cmd[i])
	{
		write(1, "\n", 1);
		return (0);
	}
	while (cmd[i] && check_flag(cmd[i]) == 1)
	{
		flag = 1;
		i++;
	}
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1] != NULL && cmd[i][0] != 0)
			printf(" ");
		i++;
	}
	if (flag == 0)
		printf("\n");
	return (0);
}
