#include "minishell.h"

// void	free_2d_array(char **str)
// {
// 	int i;

// 	i = 0;
// 	while(str[i])
// 	{
// 		free(str[i]);
// 		i++;
// 	}
// 	free(str);
// }

int total_strings(char **env)
{
	int i;

	i = 0;
	while(env[i])
		i++;
	return i;
}

void	malloc_dup_env(char **dest, char **env)
{
	int i;

	i = 0;
	while(env[i])
	{
		dest[i] = ft_strdup(env[i]);
		i++;
	}
	dest[i] = NULL;
}

void	sort_env(char ***envp)
{
	int i = 0;
	int j;
	char *temp;
	char **env = *envp;

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
	while(str[i])
	{
		if(str[i] == '=')
			flag = 1;
		if(ft_isalnum(str[i]) == 0 && flag == 0)
			return (1);
		i++;
	}
	if(flag == 0)
		return 2;
	return 0;
}

// int	check_export_args(char **str)
// {
// 	int	i;

// 	i = 0;
// 	while(str[i])
// 	{
// 		if((str[i][0] >= 0 && str[i][0] <= 9) || check_str(str[i]) == 1)
// 		{
// 			printf("Error: export: '%s': not a valid identifier\n", str[i]);
// 			return -1;
// 		}
// 		i++;
// 	}
// 	return 0;
// }

int export(char ***env, char **str)
{
	char **tempenv;
	int	i;

	i = 1;
	tempenv = malloc(sizeof(char *) * (total_strings(*env) + 2));
	
	malloc_dup_env(tempenv, *env);
	if(str[1])
	{
		while(str[i])
		{
			if(ft_isdigit(str[i][0]) == 1 || check_str(str[i]) != 0)
			{
				free_2d_array(*env);
				*env = tempenv;
				if(check_str(str[i]) != 2)
					printf("Error: export: '%s': not a valid identifier\n", str[i]);
				if(check_str(str[i]) == 2)
					return 0;
				return 1;
			}
			tempenv[total_strings(tempenv)] = ft_strdup(str[i]);
			tempenv[total_strings(tempenv) + 1] = NULL;
			i++;
		}
		free_2d_array(*env);
		*env = tempenv;
		return 0;
	}
	else
	{
		sort_env(&tempenv);
		while(tempenv[i])
		{
			printf("declare -x %s\n", tempenv[i]);
			i++;
		}
		free_2d_array(tempenv);
		return 0;
	}
}
