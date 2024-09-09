#include "minishell.h"

char	*get_expand_string(char *str, char **envp)
{
	char	*temp;
	char	*temp2;
	int i = 0;

	temp2 = ft_strjoin(str, "=");
	while(envp[i] != NULL && ft_strncmp(envp[i], temp2, ft_strlen(temp2)) != 0)
		i++;
	if (envp[i] == NULL)
		return NULL; 
	temp = ft_substr(envp[i], ft_strlen(str) + 1, ft_strlen(envp[i]));
	free(temp2);
	return temp;
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

int	copy_substr(char **res, char *substr, int i)
{
	char *str = *res;
	int	j;

	j = 0;
	if(!substr)
		return i;
	while(substr[j])
	{
		str[i] = substr[j];
		i++;
		j++;
	}
	free(substr);
	return i;
}

char*		get_substr(char *line, int i)
{
	int j;
	char	*res;

	j = i + 1;
	while(line[j] && line[j] != ' ' && line[j] != '\"' && line[j] != '$' && line[j] != '\'')
		j++;
	res = ft_substr(line, i + 1, j - (i + 1));
	return res;
}

char	*expand_string(char *line, t_data *data)
{
	int i = 0;
	char	*res = malloc(sizeof(char) * 1000000);
	char	*temp;
	int		double_quotes_flag;
	int j;

	j = 0;
	double_quotes_flag = 0;
	while(line[j])
	{
		if(line[j] == '\"')
		{
			res[i] = line[j];
			i++;
			j++;
			if(double_quotes_flag == 0)
				double_quotes_flag = 1;
			else
				double_quotes_flag = 0;
		}
		else if(line[j] == '$')
		{
			if(line[j + 1] == '\0')
			{
				res[i] = line[j];
			}
			else if(line[j + 1] == '?')
			{
				res[i] = data->exit_code + '0';
				i++;
				j += 2;
			}
			else
			{
				temp = get_expand_string(get_substr(line, j), data->envp);
				// printf("%s\n", get_substr(line, j));
				i = copy_substr(&res, temp, i);
				j++;
				while(line[j] && line[j] != ' ' && line[j] != '$' && line[j] != '\'' && line[j] != '\"')
					j++;
			}
		}
		else if(line[j] == '\'' && double_quotes_flag == 0)
		{
			res[i++] = line[j++];
			while(line[j] && line[j] != '\'')
			{
				res[i] = line[j];
				i++;
				j++;
			}
			res[i++] = line[j++];
		}
		else
		{
			res[i] = line[j];
			i++;
			j++;
		}
	}
	res[i] = '\0';
	return res;
}


void	expansion(t_tokens *list, t_data *data)
{
	t_tokens	*templist;
	
	templist = list;
	while(templist)
	{
		if(templist->type == string)
			templist->token = expand_string(templist->token, data);
		templist = templist->next;
	}
}

// int main(int argc, char **argv, char **envp)
// {
// 	char temp[] = "\"'$USER'\"";
// 	t_data data;
// 	data.envp = envp;
// 	data.exit_code = 0;
// 	char *str = expand_string(temp, &data);
// 	printf("%s\n", str);
// 	return 0;
// }