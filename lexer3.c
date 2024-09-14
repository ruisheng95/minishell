/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:00:53 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/14 19:05:07 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quotes_increment(char *line, int i)
{
	if (line[i] == '\'')
	{
		i++;
		while (line[i] && line[i] != '\'')
			i++;
		if (line[i])
			i++;
	}
	if (line[i] == '\"')
	{
		i++;
		while (line[i] && line[i] != '\"')
			i++;
		if (line[i])
			i++;
	}
	return (i);
}

char	**semi_ft_split(char *line)
{
	int		i;
	int		start;
	char	**res;
	int		j;

	i = 0;
	j = 0;
	res = malloc(sizeof(char *) * count_lexer_array(line));
	while (line[i])
	{
		while (line[i] && line[i] == ' ')
			i++;
		start = i;
		while (line[i] && (line[i] != ' '))
		{
			if (line[i] == '\'' || line[i] == '\"')
				i = handle_quotes_increment(line, i);
			else
				i++;
		}
		res[j] = ft_substr(line, start, i - start);
		j++;
	}
	free(line);
	return (res);
}

void	insert_split_tokens(t_tokens *node, char **stringsarray)
{
	t_tokens	*newnode;
	t_tokens	*lastnode;
	t_tokens	*tempnode;
	int			i;

	i = 1;
	lastnode = node;
	tempnode = node->next;
	while (stringsarray[i])
	{
		newnode = malloc(sizeof(t_tokens));
		newnode->type = string;
		newnode->token = stringsarray[i];
		newnode->prev = lastnode;
		lastnode->next = newnode;
		lastnode = newnode;
		i++;
	}
	lastnode->next = tempnode;
}

void	tokens_list_processing(t_tokens *list)
{
	t_tokens	*templist;
	char		**stringsarray;
	t_tokens	*tempnode;

	templist = list;
	while (templist)
	{
		if (templist->type == string)
		{
			stringsarray = semi_ft_split(templist->token);
			templist->token = stringsarray[0];
			if (stringsarray[1])
			{
				tempnode = templist->next;
				insert_split_tokens(templist, stringsarray);
				while (templist->next && templist->next != tempnode)
					templist = templist->next;
			}
			free(stringsarray);
		}
		templist = templist->next;
	}
}
