#include "minishell.h"

int cd(char **cmd, t_data *data)
{
	char *path;

	change_env_oldpwd(data);
	if(cmd[1] == NULL)
	{
		path = get_expand_string("HOME", data->envp);
		if(!path)
		{
			printf("Error: cd: HOME not set\n");
			return 1;
		}
	}
	else
		path = cmd[1];
	if (chdir(path) == -1)
	{
		perror("cd");
		return 1;
	}
	// printf("current dir %s\n", getcwd(NULL, 0));
	change_env_pwd(data);
	return 0;
}