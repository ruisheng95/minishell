/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 00:13:12 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/13 18:20:50 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_shell_lvl(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	i++;
	if (str[i] == '\0')
		return (1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

char	*inc_shell_lvl_helper_cuz_norm(char *str)
{
	int		i;
	int		shellvl;
	char	*res;
	char	*temp;
	char	*temp2;

	i = 0;
	while (str[i] && (str[i] < '0' || str[i] > '9'))
		i++;
	temp = ft_substr(str, i, ft_strlen(str) - i);
	shellvl = ft_atoi(temp);
	shellvl++;
	temp2 = ft_itoa(shellvl);
	res = ft_strjoin("SHLVL=", temp2);
	free(temp);
	free(temp2);
	free(str);
	return (res);
}

void	increment_shell_lvl(t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "SHLVL=", 6) == 0)
		{
			if (check_shell_lvl(data->envp[i]) == 1)
			{
				free(data->envp[i]);
				data->envp[i] = ft_strdup("SHLVL=1");
				return ;
			}
			data->envp[i] = inc_shell_lvl_helper_cuz_norm(data->envp[i]);
			return ;
		}
		i++;
	}
	data->envp[i] = ft_strdup("SHLVL=1");
	data->envp[i + 1] = NULL;
	return ;
}

void	change_env_oldpwd(t_data *data)
{
	char	*oldpwd;
	char	*newstr;
	int		i;

	i = 0;
	oldpwd = getcwd(NULL, 0);
	newstr = ft_strjoin("OLDPWD=", oldpwd);
	free(oldpwd);
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "OLDPWD=", 7) == 0)
		{
			free(data->envp[i]);
			data->envp[i] = newstr;
			return ;
		}
		i++;
	}
	if (!data->envp[i])
		free(newstr);
	return ;
}

void	change_env_pwd(t_data *data)
{
	char	*pwd;
	char	*newstr;
	int		i;

	i = 0;
	pwd = getcwd(NULL, 0);
	newstr = ft_strjoin("PWD=", pwd);
	free(pwd);
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "PWD=", 4) == 0)
		{
			free(data->envp[i]);
			data->envp[i] = newstr;
			return ;
		}
		i++;
	}
	if (!data->envp[i])
		free(newstr);
	return ;
}
