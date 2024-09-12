/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 00:39:43 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/13 00:42:28 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(char **cmd, t_data *data)
{
	char	*path;

	change_env_oldpwd(data);
	if (cmd[1] == NULL || cmd[1][0] == '\0')
	{
		path = get_expand_string("HOME", data->envp);
		if (!path)
		{
			printf("Error: cd: HOME not set\n");
			return (1);
		}
	}
	else
		path = cmd[1];
	if (chdir(path) == -1)
	{
		perror("cd");
		return (1);
	}
	change_env_pwd(data);
	return (0);
}
