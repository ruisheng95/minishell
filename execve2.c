/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rng <rng@student.42kl.edu.my>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 18:28:52 by rng               #+#    #+#             */
/*   Updated: 2024/09/15 20:48:00 by rng              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_non_std_fd(int ignore_fd_1, int ignore_fd_2)
{
	int	i;

	i = 3;
	while (i < 1000)
	{
		if (i != ignore_fd_1 && i != ignore_fd_2)
			close(i);
		i++;
	}
}

void	prepare_fd(t_cmd_list *templist, t_data *data)
{
	dup2(templist->in_fd, 0);
	dup2(templist->out_fd, 1);
	close_non_std_fd(data->saved_in_fd, data->saved_out_fd);
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
