#include "minishell.h"

void	exit_error(int n)
{
	if (n == 1)
		write(2, "Error: wrong pipex usage\n", 25);
	else
		perror("Error");
	exit(1);
}

void	exit_error_str(char *str, int n)
{
	int	len;

	len = ft_strlen(str);
	if (n == 1)
		write(2, "Error: command not found: ", 26);
	else
		write(2, "Error: no such file or directory: ", 35);
	write(2, str, len);
	write(2, "\n", 1);
	exit(1);
}

char	*get_path(char **envp, char *cmd)
{
	int		i;
	char	**paths_list;
	char	*paths_string;
	char	*full_path;
	char	*temp;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH", 4) != 0)
		i++;
	if(!envp[i])
	{
		write(2, "Error: ", 7);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": No such file or directory\n", 29);
		return NULL;
	}
	paths_string = ft_substr(envp[i], 5, ft_strlen(envp[i]));
	paths_list = ft_split(paths_string, ':');
	i = -1;
	while (paths_list[++i])
	{
		temp = ft_strjoin(paths_list[i], "/");
		full_path = ft_strjoin(temp, cmd);
		if (access(full_path, F_OK) == 0)
			return (full_path);
		free(temp);
		free(full_path);
	}
	free_2d_array(paths_list);
	exit_error_str(cmd, 1);
	return (NULL);
}

int buildins(char **cmd, t_data **data)
{
	if(ft_strcmp(cmd[0], "cd") == 0)
	{
		return(cd(cmd, (*data)->envp));
	}
	if(ft_strcmp(cmd[0], "export") == 0)
	{
		return(export(&(*data)->envp, cmd));
	}
	if(ft_strcmp(cmd[0], "unset") == 0)
	{
		return(unset(&(*data)->envp, cmd[1]));
	}
	if(ft_strcmp(cmd[0], "env") == 0)
	{
		return(env(*data));
	}
	if(ft_strcmp(cmd[0], "pwd") == 0)
	{
		return(pwd());
	}
	if(ft_strcmp(cmd[0], "echo") == 0)
	{
		return(echo(cmd));
	}
	if(ft_strcmp(cmd[0], "echo") == 0)
	{
		return(echo(cmd));
	}
	if(ft_strcmp(cmd[0], "exit") == 0)
	{
		return(exit_process());
	}
	return 1;
}
int	execute(char **cmd, t_data **data)
{
	char	*path;
	int		exit_status;
	pid_t	pid;

	if (ft_strcmp(cmd[0], "echo") == 0 ||
		ft_strcmp(cmd[0], "pwd") == 0 ||
		ft_strcmp(cmd[0], "env") == 0 ||
		ft_strcmp(cmd[0], "unset") == 0 ||
		ft_strcmp(cmd[0], "export") == 0 ||
		ft_strcmp(cmd[0], "cd") == 0 ||
		ft_strcmp(cmd[0], "exit") == 0)
		{
			return(buildins(cmd, data));
		}
	pid = fork();
	if (cmd == NULL)
		exit_error(0);
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		set_terminos_echo(1);
		// cmd = lexer(argv, envp);
		if (cmd[0][0] == '.' && cmd[0][1] == '/')
			path = ft_strdup(cmd[0]);
		else if(cmd[0][0] == '/')
			path = ft_strdup(cmd[0]);
		else
			path = get_path((*data)->envp, cmd[0]);
		if(!path)
			exit(1);
		return(execve(path, cmd, (*data)->envp));
	}
	else
		waitpid(pid, &exit_status, 0);
	// if (execve(path, cmd, envp) == -1)
	// 	exit_error(0);
	if (WIFSIGNALED(exit_status) && WTERMSIG(exit_status) == SIGQUIT)
		ft_printf("Quit\n");
	else if (WIFSIGNALED(exit_status) && WTERMSIG(exit_status) == SIGINT)
		ft_printf("\n");
	return(exit_status);
}