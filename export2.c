/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 21:39:00 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/14 21:39:51 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	total_strings(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

void	malloc_dup_env(char **dest, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		dest[i] = ft_strdup(env[i]);
		i++;
	}
	dest[i] = NULL;
}

void	sort_env(char ***envp)
{
	int		i;
	int		j;
	char	*temp;
	char	**env;

	env = *envp;
	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				temp = env[i];
				env[i] = env[j];
				env[j] = temp;
			}
			j++;
		}
		i++;
	}
}

int	check_str(char *str)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (str[i])
	{
		if (str[i] == '=')
			flag = 1;
		if (ft_isalnum(str[i]) == 0 && flag == 0)
			return (1);
		i++;
	}
	if (flag == 0)
		return (2);
	return (0);
}

int	print_sorted_env(char **tempenv)
{
	int	i;

	i = 0;
	sort_env(&tempenv);
	while (tempenv[i])
	{
		printf("declare -x %s\n", tempenv[i]);
		i++;
	}
	free_2d_array(tempenv);
	return (0);
}
