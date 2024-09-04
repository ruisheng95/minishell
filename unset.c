#include "minishell.h"

char **unset_array(char **env, char *str)
{
	char **tempenv;
	char	*s;
	int i;
	int j;

	i = 0;
	j = 0;
	tempenv = malloc((total_strings(env) + 1) * sizeof(char *));
	s = ft_strjoin(str, "=");
	if(!tempenv)
		return NULL;
	while(env[i])
	{
		if(ft_strncmp(env[i], s, ft_strlen(s)) != 0)
		{
			tempenv[j] = ft_strdup(env[i]);
			j++;
		}
		i++;
	}
	tempenv[j] = NULL;
	return tempenv;
}

int unset(char ***env, char **str)
{
	char	**tempenv;
	int		i;

	i = 0;
	if(!str)
		return 0;
	while(str[i])
	{
		tempenv = unset_array(*env, str[i]);
		free_2d_array(*env);
		*env = tempenv;
		i++;
	}
	// print_env(*env);
	return 0;
}

void	print_env(char **env)
{
	int i;

	i = 0;
	while(env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

// int main(int argc, char **argv, char **envp)
// {
// 	char **env;

// 	env = envp;
// 	print_env(env);
// 	unset(&env, "USER");
// 	printf("---------------------------------------------\n");
// 	print_env(env);
// }