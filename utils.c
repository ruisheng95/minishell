#include "minishell.h"
#include <termios.h>

void	free_2d_array(char **str)
{
	int	i;

	i = 0;
	if(!str)
		return ;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	print_2d_array(char **cmd)
{
	int i = 0;
	while(cmd[i])
	{
		printf("token %d = %s\n", i, cmd[i]);
		i++;
	}
}

int	exit_process()
{
	write(1,"exit\n", 5);
	exit(0);
	return 0;
}

int	set_terminos_echo(int enable)
{
	struct termios	terminos_p;

	if (tcgetattr(STDOUT_FILENO, &terminos_p) == -1)
		return (-1);
	if (enable)
		terminos_p.c_lflag |= ECHOCTL;
	else
	 	terminos_p.c_lflag &= ~(ECHOCTL);
	if (tcsetattr(STDOUT_FILENO, TCSANOW, &terminos_p) == -1)
		return (-1);
	return (0);
}

void	increment_shell_lvl(t_data *data)
{
	int	i;
	int	j;
	int shelllvl;

	i = 0;
	j = 0;
	while(data->envp[i])
	{
		if(ft_strncmp(data->envp[i], "SHLVL=", 6) == 0)
		{
			while(data->envp[i][j] < '0' || data->envp[i][j] > '9')
				j++;
			if(data->envp[i][j])
			{
				shelllvl = ft_atoi(ft_substr(data->envp[i], j, ft_strlen(data->envp[i]) - j));
				shelllvl++;
				free(data->envp[i]);
				data->envp[i] = ft_strjoin("SHLVL=", ft_itoa(shelllvl));
			}
			return;
		}
		i++;
	}
	data->envp[i] = ft_strdup("SHLVL=1");
	data->envp[i + 1] = NULL;
	return ;
}

void	change_env_oldpwd(t_data *data)
{
	char	*oldpwd;
	char	*newstr;
	int		i;

	i = 0;
	oldpwd = getcwd(NULL, 0);
	newstr = ft_strjoin("OLDPWD=", oldpwd);
	free(oldpwd);
	while(data->envp[i])
	{
		if(ft_strncmp(data->envp[i], "OLDPWD=", 7) == 0)
		{
			free(data->envp[i]);
			data->envp[i] = newstr;
			return ;
		}
		i++;
	}
	if(!data->envp[i])
		free(newstr);
	return ;
}

void	change_env_pwd(t_data *data)
{
	char	*pwd;
	char	*newstr;
	int		i;

	i = 0;
	pwd = getcwd(NULL, 0);
	newstr = ft_strjoin("PWD=", pwd);
	free(pwd);
	while(data->envp[i])
	{
		if(ft_strncmp(data->envp[i], "PWD=", 4) == 0)
		{
			free(data->envp[i]);
			data->envp[i] = newstr;
			return ;
		}
		i++;
	}
	if(!data->envp[i])
		free(newstr);
	return ;
}

void	free_t_tokens(t_tokens *obj)
{
	t_tokens	*node;
	t_tokens	*temp_node;

	if (!obj)
		return ;
	node = obj;
	while (node)
	{
		temp_node = node;
		node = node->next;
		free(temp_node);
	}
}

void	free_t_node(t_node *obj)
{
	t_node	*node;
	t_node	*temp_node;
	int		i;

	if (!obj)
		return ;
	node = obj;
	while (node)
	{
		temp_node = node;
		node = node->next;
		if(temp_node->type == s_command)
		{
			free(temp_node->simple_cmd.cmd);
			i = -1;
			while (temp_node->simple_cmd.array[++i])
				free(temp_node->simple_cmd.array[i]);
			free(temp_node->simple_cmd.array);
		}
		free(temp_node);
	}
}

void	free_t_cmd_list(t_cmd_list *obj)
{
	t_cmd_list	*node;
	t_cmd_list	*temp_node;

	if (!obj)
		return ;
	node = obj;
	while (node)
	{
		temp_node = node;
		node = node->next;
		if(ft_strcmp(temp_node->cmd[0], heredoc_cmd) == 0)
		{
			free(temp_node->cmd[0]);
			free(temp_node->cmd);
		}
		free(temp_node);
	}
}