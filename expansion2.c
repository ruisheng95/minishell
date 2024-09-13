/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:47:30 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/13 16:48:31 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_expand_string(char *str, char **envp)
{
	char	*temp;
	char	*temp2;
	int		i;

	i = 0;
	temp2 = ft_strjoin(str, "=");
	while (envp[i] != NULL && ft_strncmp(envp[i], temp2, ft_strlen(temp2)) != 0)
		i++;
	if (envp[i] == NULL)
		return (NULL);
	temp = ft_substr(envp[i], ft_strlen(str) + 1, ft_strlen(envp[i]));
	free(temp2);
	return (temp);
}

int	copy_substr(char **res, char *substr, int i)
{
	char	*str;
	int		j;

	j = 0;
	str = *res;
	if (!substr)
		return (i);
	while (substr[j])
	{
		str[i] = substr[j];
		i++;
		j++;
	}
	free(substr);
	return (i);
}

char	*get_substr(char *line, int i)
{
	int		j;
	char	*res;

	j = i + 1;
	while (line[j] && line[j] != ' ' && line[j] != '\"'
		&& line[j] != '$' && line[j] != '\'')
		j++;
	res = ft_substr(line, i + 1, j - (i + 1));
	return (res);
}
