/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:05:09 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/14 19:05:19 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_quotes_helper_cuz_norminette(char *line,
	char **res, int *i, int *j)
{
	char	quote;

	quote = line[(*i)++];
	while (line[(*i)] && line[(*i)] != quote)
		(*res)[(*j)++] = line[(*i)++];
	if (line[(*i)])
		(*i)++;
}

char	*remove_quotes(char *line)
{
	char	*res;
	int		i;
	int		j;

	if (!line || line[0] == '\0')
		return (NULL);
	i = 0;
	j = 0;
	res = malloc(sizeof(char) * ft_strlen(line) + 1);
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
			remove_quotes_helper_cuz_norminette(line, &res, &i, &j);
		else
			res[j++] = line[i++];
	}
	res[j] = '\0';
	return (res);
}

void	remove_quotes_from_token_list(t_tokens *list)
{
	t_tokens	*templist;
	char		*tempptr;

	templist = list;
	while (templist)
	{
		if (templist->type == string)
		{
			tempptr = templist->token;
			templist->token = remove_quotes(templist->token);
			free(tempptr);
		}
		templist = templist->next;
	}
}
