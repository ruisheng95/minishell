/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rng <rng@student.42kl.edu.my>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 18:43:18 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/17 22:57:18 by rng              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	here_doc_gnl(char *lim);
void	prepare_fd_and_signal(t_cmd_list *templist, t_data *data);
void	execve_error(void);

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
		exit(126);
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

char	*get_command_path(char *cmd, t_data *data)
{
	if (cmd[0] == '.' && cmd[1] == '/')
		return (ft_strdup(cmd));
	else if (cmd[0] == '/')
		return (ft_strdup(cmd));
	else
		return (get_path(data->envp, cmd));
}

int	execute(char **cmd, t_data **data, t_cmd_list *templist)
{
	char	*path;
	pid_t	pid;
	int		exit_status;

	if (cmd == NULL || cmd[0][0] == '\0')
		return (0);
	if (check_buildins_call(cmd[0]) == 1)
		return (buildins(cmd, data, templist));
	pid = fork();
	if (pid == 0)
	{
		prepare_fd_and_signal(templist, *data);
		if (ft_strcmp(cmd[0], HEREDOC_CMD) == 0)
			return (here_doc_gnl(cmd[1]), exit(0), 0);
		path = get_command_path(cmd[0], *data);
		if (execve(path, cmd, (*data)->envp) == -1)
			execve_error();
	}
	else
	{
		templist->pid = pid;
		if (ft_strcmp(cmd[0], HEREDOC_CMD) == 0)
			waitpid(templist->pid, &exit_status, 0);
	}
	return (0);
}
