/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rng <rng@student.42kl.edu.my>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:51:35 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/15 22:05:12 by rng              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_string_helper_cuz_norminette_3(char **res, int *i, int exit_code)
{
	int		k;
	char	*temp;

	k = -1;
	temp = ft_itoa(exit_code);
	while (temp[++k])
		(*res)[(*i)++] = temp[k];
	free(temp);
}

int	expand_string_helper_cuz_norminette(char **res, char *line,
	int *i, t_data *data)
{
	int		j;
	char	*temp;

	j = 0;
	if (line[j + 1] == '\0' || line[j + 1] == '\"'
		|| line [j + 1] == '\'' || line[j + 1] == '\"' || line[j + 1] == ' ')
		(*res)[(*i)++] = '$';
	else if (line[j + 1] == '?')
	{
		expand_string_helper_cuz_norminette_3(res, i, data->exit_code);
		j++;
	}
	else
	{
		temp = get_expand_string(get_substr(line, j), data->envp);
		(*i) = copy_substr(&(*res), temp, (*i));
		j++;
		while (line[j] && line[j] != ' '
			&& line[j] != '$' && line[j] != '\'' && line[j] != '\"')
			j++;
		j--;
	}
	j++;
	return (j);
}

void	expand_string_helper_cuz_norminette_2(char **res, char *line,
	int *i, int *j)
{
	(*res)[(*i)++] = line[(*j)++];
	while (line[(*j)] && line[(*j)] != '\'')
		(*res)[(*i)++] = line[(*j)++];
	(*res)[(*i)++] = line[(*j)++];
}

char	*expand_string(char *line, t_data *data)
{
	int		i;
	char	*res;
	int		double_quotes_flag;
	int		j;

	j = 0;
	i = 0;
	res = malloc(sizeof(char) * 1000000);
	double_quotes_flag = 0;
	while (line[j])
	{
		if (line[j] == '\"')
		{
			res[i++] = line[j++];
			double_quotes_flag = !(double_quotes_flag);
		}
		else if (line[j] == '$')
			j += expand_string_helper_cuz_norminette(&res, line + j, &i, data);
		else if (line[j] == '\'' && double_quotes_flag == 0)
			expand_string_helper_cuz_norminette_2(&res, line, &i, &j);
		else
			res[i++] = line[j++];
	}
	res[i] = '\0';
	return (res);
}

void	expansion(t_tokens *list, t_data *data)
{
	t_tokens	*templist;

	templist = list;
	while (templist)
	{
		if (templist->type == STRING)
			templist->token = expand_string(templist->token, data);
		templist = templist->next;
	}
}

// char	*expansion(char *line, t_lexing *lexer)
// {
// 	int i;
// 	int start;
// 	char *res;
// 	char *temp;
// 	int j;
// 	int k = 0;
// 	char *tmp;

// 	i = 0;
// 	j = 0;
// 	res = malloc(sizeof(char) * 1000000);
// 	while (line[j])
// 	{
// 		if (line[j] == '$')
// 		{
// 			start = j + 1;
// 			if(line[start] == '\0')
// 				res[i] = '$';
// 			else if(line[start] == '?')
// 			{
// 				while(line[j] == '$' && line[j + 1] == '?')
// 				{
// 					res[i] = lexer->exit_code + 48;
// 					i++;
// 					j += 2;
// 				}
// 			}
// 			else
// 			{
// 				while(line[j] && line[j] != ' ')
// 					j++;
// 				tmp = ft_substr(line, start, j - start);
// 				temp = get_expand_string(tmp, lexer->envp);
// 				free(tmp);
// 				if (temp != NULL)
// 				{
// 					k = 0;
// 					while(temp[k])
// 					{
// 						res[i] = temp[k];
// 						i++;
// 						k++;
// 					}
// 					free(temp);
// 				}
// 			}
// 		}
// 		if (line[j])
// 			res[i++] = line[j++];
// 	}
// 	res[i] = '\0';
// 	return res;
// }

// int main(int argc, char **argv, char **envp)
// {
// 	char temp[] = "$invalid";
// 	t_data data;
// 	data.envp = envp;
// 	data.exit_code = 0;
// 	char *str = expand_string(temp, &data);
// 	printf("%s\n", str);
// 	return 0;
// }