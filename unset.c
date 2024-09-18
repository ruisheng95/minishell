/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rng <rng@student.42kl.edu.my>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:45:55 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/18 08:49:53 by rng              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**unset_array(char **env, char *str)
{
	char	**tempenv;
	char	*s;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tempenv = malloc((total_strings(env) + 1) * sizeof(char *));
	if (!tempenv)
		return (NULL);
	s = ft_strjoin(str, "=");
	while (env[i])
	{
		if (ft_strncmp(env[i], s, ft_strlen(s)) != 0)
		{
			tempenv[j] = ft_strdup(env[i]);
			j++;
		}
		i++;
	}
	free(s);
	tempenv[j] = NULL;
	return (tempenv);
}

int	unset(char ***env, char **str)
{
	char	**tempenv;
	int		i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		tempenv = unset_array(*env, str[i]);
		free_2d_array(*env);
		*env = tempenv;
		i++;
	}
	return (0);
}

// void	print_env(char **env)
// {
// 	int i;

// 	i = 0;
// 	while (env[i])
// 	{
// 		printf("%s\n", env[i]);
// 		i++;
// 	}
// }

// int main(int argc, char **argv, char **envp)
// {
// 	char **env;

// 	env = envp;
// 	print_env(env);
// 	unset(&env, "USER");
// 	printf("---------------------------------------------\n");
// 	print_env(env);
// }