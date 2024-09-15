/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 18:43:18 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/16 00:28:06 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prepare_fd(t_cmd_list *templist, t_data *data);
void	here_doc_gnl(char *lim);

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
	if (n == 1)
		exit(127);
	exit(1);
}

char	*get_path_helper_cuz_norm(char **envp, char *cmd)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH", 4) != 0)
		i++;
	if (!envp[i])
	{
		write(2, "Error: ", 7);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": No such file or directory\n", 29);
		return (NULL);
	}
	return (ft_substr(envp[i], 5, ft_strlen(envp[i])));
}

char	*get_path(char **envp, char *cmd)
{
	int		i;
	char	**paths_list;
	char	*paths_string;
	char	*full_path;
	char	*temp;

	paths_string = get_path_helper_cuz_norm(envp, cmd);
	if (!paths_string)
		return (NULL);
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

int	check_buildins_call(char *str)
{
	if (ft_strcmp(str, "echo") == 0
		|| ft_strcmp(str, "pwd") == 0
		|| ft_strcmp(str, "env") == 0
		|| ft_strcmp(str, "unset") == 0
		|| ft_strcmp(str, "export") == 0
		|| ft_strcmp(str, "cd") == 0
		|| ft_strcmp(str, "exit") == 0)
		return (1);
	return (0);
}

void	prepare_buildins_fd(t_cmd_list *templist)
{
	dup2(templist->in_fd, 0);
	dup2(templist->out_fd, 1);
	if (templist->in_fd != 0)
		close(templist->in_fd);
	if (templist->out_fd != 1)
		close(templist->out_fd);
}

int	buildins(char **cmd, t_data **data, t_cmd_list *templist)
{
	int	n;

	n = 0;
	prepare_buildins_fd(templist);
	if (ft_strcmp(cmd[0], "cd") == 0)
		n = cd(cmd, *data);
	if (ft_strcmp(cmd[0], "export") == 0)
		n = export(&(*data)->envp, cmd);
	if (ft_strcmp(cmd[0], "unset") == 0)
		n = unset(&(*data)->envp, cmd);
	if (ft_strcmp(cmd[0], "env") == 0)
		n = env(*data);
	if (ft_strcmp(cmd[0], "pwd") == 0)
		n = pwd();
	if (ft_strcmp(cmd[0], "echo") == 0)
		n = echo(cmd);
	if (ft_strcmp(cmd[0], "exit") == 0)
		n = exit_process(cmd, *data);
	dup2((*data)->saved_in_fd, 0);
	dup2((*data)->saved_out_fd, 1);
	return (n);
}

int	execute(char **cmd, t_data **data, t_cmd_list *templist)
{
	char	*path;
	pid_t	pid;
	int		exit_status;

	if (cmd == NULL || cmd[0][0] == '\0')
		return(0);
	if (check_buildins_call(cmd[0]) == 1)
		return(buildins(cmd, data, templist));
	pid = fork();
	if (pid == 0)
	{
		prepare_fd(templist, *data);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		set_terminos_echo(1);
		if (ft_strcmp(cmd[0], HEREDOC_CMD) == 0)
			return (here_doc_gnl(cmd[1]), exit(0), 0);
		if (cmd[0][0] == '.' && cmd[0][1] == '/')
			path = ft_strdup(cmd[0]);
		else if(cmd[0][0] == '/')
			path = ft_strdup(cmd[0]);
		else
			path = get_path((*data)->envp, cmd[0]);
		if(execve(path, cmd, (*data)->envp) == -1)
		{
			perror("execve: ");
			exit (126);
		}
	}
	else
	{
		templist->pid = pid;
		if (ft_strcmp(cmd[0], HEREDOC_CMD) == 0)
			waitpid(templist->pid, &exit_status, 0);
	}
	return(0);
}
