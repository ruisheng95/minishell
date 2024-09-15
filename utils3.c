/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 01:18:55 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/16 01:27:08 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_exit_code(t_data *data)
{
	data->exit_code = 1;
	return (1);
}

void	init_data_struct(t_data *data, char **envp)
{
	data->envp = malloc(sizeof(char *) * (total_strings(envp) + 1));
	malloc_dup_env(data->envp, envp);
	increment_shell_lvl(data);
	data->saved_in_fd = dup(0);
	data->saved_out_fd = dup(1);
	data->exit_code = 0;
}
