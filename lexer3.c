/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rng <rng@student.42kl.edu.my>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:00:53 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/15 10:38:43 by rng              ###   ########.fr       */
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
	res = ft_calloc(sizeof(char *), count_lexer_array(line));
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
		newnode->type = STRING;
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
		if (templist->type == STRING)
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

void	print_tokens_list(t_tokens *list)
{
	int	i;

	i = 0;
	while (list != NULL)
	{
		printf("token %d : %s, type : %d\n", i, list->token, list->type);
		list = list->next;
		i++;
	}
}
