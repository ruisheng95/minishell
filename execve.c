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
	int	n;
	if(ft_strcmp(cmd[0], "cd") == 0)
	{
		return(cd(cmd, *data));
	}
	if(ft_strcmp(cmd[0], "export") == 0)
	{
		return(export(&(*data)->envp, cmd));
	}
	if(ft_strcmp(cmd[0], "unset") == 0)
	{
		return(unset(&(*data)->envp, cmd));
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

void	prepare_fd(t_cmd_list *templist)
{
	dup2(templist->in_fd, 0);
	dup2(templist->out_fd, 1);
	for (int i = 3; i < 1000; i++)
		close(i);
}

void	here_doc_gnl(char *lim)
{
	char	*str;
	size_t	len;

	str = get_next_line(STDIN_FILENO);
	while (str)
	{
		len = ft_strlen(lim);
		if (ft_strlen(str) - 1 > len)
			len = ft_strlen(str);
		if (!ft_strncmp(str, lim, len))
			return (free(str));
		write(STDOUT_FILENO, str, ft_strlen(str));
		free(str);
		str = get_next_line(STDIN_FILENO);
	}
}

int	execute(char **cmd, t_data **data, t_cmd_list *templist)
{
	char	*path;
	pid_t	pid;
	int		exit_status;

	if (cmd == NULL)
		exit_error(0);
	pid = fork();
	if (pid == 0)
	{
		prepare_fd(templist);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		set_terminos_echo(1);
		if (ft_strcmp(cmd[0], heredoc_cmd) == 0)
			return (here_doc_gnl(cmd[1]), exit(0), 0);
		if (ft_strcmp(cmd[0], "echo") == 0 ||
			ft_strcmp(cmd[0], "pwd") == 0 ||
			ft_strcmp(cmd[0], "env") == 0 ||
			ft_strcmp(cmd[0], "unset") == 0 ||
			ft_strcmp(cmd[0], "export") == 0 ||
			ft_strcmp(cmd[0], "cd") == 0 ||
			ft_strcmp(cmd[0], "exit") == 0)
			return (buildins(cmd, data), exit(0), 0);
		if (cmd[0][0] == '.' && cmd[0][1] == '/')
		{
			path = ft_strdup(cmd[0]);
		}
		else if(cmd[0][0] == '/')
			path = ft_strdup(cmd[0]);
		else
			path = get_path((*data)->envp, cmd[0]);
		if(!path)
			exit(1);
		execve(path, cmd, (*data)->envp);
	}
	else
	{
		templist->pid = pid;
		if (ft_strcmp(cmd[0], heredoc_cmd) == 0)
			waitpid(templist->pid, &exit_status, 0);
	}
	return(0);
}