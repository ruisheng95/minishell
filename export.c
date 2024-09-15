/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:18:56 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/16 00:15:16 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	export_helper_cuz_norminette(char *str, char ***tempenv)
{
	int		j;
	char	*tempsubstr;

	j = 0;
	while (str[j] && str[j] != '=')
		j++;
	tempsubstr = ft_substr(str, 0, j + 1);
	j = find_export_index((*tempenv), tempsubstr);
	if ((*tempenv)[j] == NULL)
	{
		free((*tempenv)[j]);
		(*tempenv)[j] = ft_strdup(str);
		(*tempenv)[j + 1] = NULL;
	}
	else
		(*tempenv)[j] = ft_strdup(str);
	free(tempsubstr);
}

int	export_valid_check(char *str)
{
	if (ft_isdigit(str[0]) == 1 || check_str(str) != 0 || str[0] == '=')
	{
		if (check_str(str) != 2)
			printf("Error: export: '%s': not a valid identifier\n", str);
		if (check_str(str) == 2 && ft_isdigit(str[0]) == 0)
			return (2);
		return (1);
	}
	return (0);
}

int	export(char ***env, char **str)
{
	char	**tempenv;
	int		i;
	int		error;

	i = 0;
	tempenv = malloc(sizeof(char *) * (total_strings(*env) + 2));
	malloc_dup_env(tempenv, *env);
	if (!str[1])
		return (print_sorted_env(tempenv));
	while (str[++i])
	{
		error = export_valid_check(str[i]);
		if (error != 0)
		{
			free_2d_array(*env);
			*env = tempenv;
			if (error == 2)
				return (0);
			return (1);
		}
		export_helper_cuz_norminette(str[i], &tempenv);
	}
	free_2d_array(*env);
	*env = tempenv;
	return (0);
}
