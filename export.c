/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:18:56 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/13 17:50:57 by ethanlim         ###   ########.fr       */
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

int	find_export_index(char **env, char *str)
{
	char	**tempenv;
	int		i;

	tempenv = env;
	i = 0;
	while (tempenv[i] && ft_strncmp(tempenv[i], str, ft_strlen(str)) != 0)
		i++;
	return (i);
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

// int	export_helper_cuz_norminette()
// {

// }

int	export(char ***env, char **str)
{
	char	**tempenv;
	char	*tempsubstr;
	int		i;
	int		j;

	i = 1;
	tempsubstr = NULL;
	tempenv = malloc(sizeof(char *) * (total_strings(*env) + 2));
	if (!tempenv)
		return (1);
	malloc_dup_env(tempenv, *env);
	if (!str[1])
		return (print_sorted_env(tempenv));
	while (str[i])
	{
		if (ft_isdigit(str[i][0]) == 1 || check_str(str[i]) != 0)
		{
			if (tempsubstr)
				free(tempsubstr);
			free_2d_array(*env);
			*env = tempenv;
			if (check_str(str[i]) != 2)
				printf("Error: export: '%s': not a valid identifier\n", str[i]);
			if (check_str(str[i]) == 2)
				return (0);
			return (1);
		}
		j = 0;
		while (str[i][j] && str[i][j] != '=')
			j++;
		tempsubstr = ft_substr(str[i], 0, j + 1);
		j = find_export_index(tempenv, tempsubstr);
		if (tempenv[j] == NULL)
		{
			free(tempenv[j]);
			tempenv[j] = ft_strdup(str[i]);
			tempenv[j + 1] = NULL;
		}
		else
			tempenv[j] = ft_strdup(str[i]);
		i++;
	}
	if (tempsubstr)
		free(tempsubstr);
	free_2d_array(*env);
	*env = tempenv;
	return (0);
}
