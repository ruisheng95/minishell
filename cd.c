#include "minishell.h"

int cd(char **cmd, char **envp)
{
	char *path;

	if(cmd[1] == NULL)
	{
		path = get_expand_string("HOME", envp);
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
	return 0;
}