#include "minishell.h"

char	**semi_ft_split(char *line)
{
	int	i;
	int	start;
	char	**res;
	int	j;

	i = 0;
	j = 0;
	res = malloc(sizeof(char *) * count_lexer_array(line));
	while(line[i])
	{
		while(line[i] && line[i] == ' ')
			i++;
		start = i;
		while(line[i] && (line[i] != ' '))
		{
			if(line[i] == '\'')
			{
				i++;
				while(line[i] && line[i] != '\'')
					i++;
				if(line[i])
					i++;
			}
			if(line[i] == '\"')
			{
				i++;
				while(line[i] && line[i] != '\"')
					i++;
				if(line[i])
					i++;
			}
			else
				i++;
		}
		res[j] = ft_substr(line, start, i - start);
		j++;
	}
	return res;
}
void	tokens_list_processing(t_tokens *list)
{
	t_tokens	*templist;
	t_tokens	*newnode;
	t_tokens	*lastnode;
	t_tokens	*tempnode;
	char	**stringsarray;
	int		i;

	templist = list;
	while(templist)
	{
		if(templist->type == string)
		{
			i = 1;
			stringsarray = semi_ft_split(templist->token);
			templist->token = stringsarray[0];
			lastnode = templist;
			tempnode = templist->next;
			if(stringsarray[1])
			{
				while(stringsarray[i])
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
				templist = lastnode;
			}
			free(stringsarray);
		}
		templist = templist->next;
	}
}

char	*remove_quotes(char *line)
{
	char	*res;
	int		i;
	int		j;

	if(!line || line[0] == '\0')
		return NULL;
	i = 0;
	j = 0;
	res = malloc(sizeof(char) * ft_strlen(line) + 1);
	while(line[i])
	{
		if(line[i] == '\'')
		{
			i++;
			while(line[i] && line[i] != '\'')
			{
				res[j] = line[i];
				i++;
				j++;
			}
			i++;
		}
		else if(line[i] == '\"')
		{
			i++;
			while(line[i] && line[i] != '\"')
			{
				res[j] = line[i];
				i++;
				j++;
			}
			i++;
		}
		else
		{
			res[j] = line[i];
			i++;
			j++;
		}
	}
	res[j] = '\0';
	return (res);
}

void	remove_quotes_from_token_list(t_tokens *list)
{
	t_tokens	*templist;
	char		*tempptr;

	templist = list;
	while(templist)
	{
		if(templist->type == string)
		{
			tempptr = templist->token;
			templist->token = remove_quotes(templist->token);
			free(tempptr);
		}
		templist = templist->next;
	}
}
