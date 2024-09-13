/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 21:56:01 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/13 22:06:41 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int handle_quotes(char *line, int i)
// {
// 	if (line[i] == '\'')
// 	{
// 		i++;
// 		while (line[i] && line[i] != '\'')
// 			i++;
// 		if (line[i] == '\0')
// 		{
// 			printf("Unclosed quote detected\n");
// 			return -1;
// 		}
// 	}
// 	else if (line[i] == '\"')
// 	{
// 		i++;
// 		while (line[i] && line[i] != '\"')
// 			i++;
// 		if (line[i] == '\0')
// 		{
// 			printf("Unclosed quote detected\n");
// 			return -1;
// 		}
// 	}
// 	i--;
// 	return i;
// }

// char	*substr_expand(char *line, t_lexing *lexer, int i, int start)
// {
// 	char *res;
// 	char *temp;

// 	temp = ft_substr(line, start, i - start);
// 	res = expansion(temp, lexer);
// 	free(temp);
// 	return res;
// }
// int	lexer_helper1(t_lexing *lexer)
// {
// 	lexer->start++;
// 	if(lexer->line[lexer->i] == '\'')
// 	{
// 		if(handle_quotes(lexer->line, lexer->i) == -1)
// 			return -1;
// 		else
// 			lexer->i = handle_quotes(lexer->line, lexer->i) + 1;
// 		lexer->res[lexer->j] = ft_substr(lexer->line, lexer->start,
// 		lexer->i - lexer->start);
// 	}
// 	else
// 	{
// 		if(handle_quotes(lexer->line, lexer->i) == -1)
// 			return -1;
// 		else
// 			lexer->i = handle_quotes(lexer->line, lexer->i) + 1;
// 		lexer->res[lexer->j] = substr_expand(lexer->line,
//		lexer, lexer->i, lexer->start);
// 	}
// 	return 0;
// }

int	lexer_helper2(t_lexing *lexer)
{
	if ((lexer->line[lexer->i] == '>' && lexer->line[lexer->i + 1] == '>')
		|| (lexer->line[lexer->i] == '<' && lexer->line[lexer->i + 1] == '<'))
	{
		lexer->res[lexer->j] = ft_substr(lexer->line, lexer->i, 2);
		lexer->i += 2;
	}
	else
	{
		lexer->res[lexer->j] = ft_substr(lexer->line, lexer->i, 1);
		lexer->i++;
	}
	return (0);
}

int	print_unclosed_quotes(void)
{
	printf("unclosed quote detected\n");
	return (-1);
}

int	lexer_helper3(t_lexing *lexer)
{
	while (lexer->line[lexer->i] && lexer->line[lexer->i] != ' '
		&& lexer->line[lexer->i] != '|' && lexer->line[lexer->i] != '>'
		&& lexer->line[lexer->i] != '<')
	{
		if (lexer->line[lexer->i] == '\'')
		{
			lexer->i++;
			while (lexer->line[lexer->i] && lexer->line[lexer->i] != '\'')
				lexer->i++;
			if (lexer->line[lexer->i] == '\0')
				return (print_unclosed_quotes());
		}
		if (lexer->line[lexer->i] == '\"')
		{
			lexer->i++;
			while (lexer->line[lexer->i] && lexer->line[lexer->i] != '\"')
				lexer->i++;
			if (lexer->line[lexer->i] == '\0')
				return (print_unclosed_quotes());
		}
		lexer->i++;
	}
	lexer->res[lexer->j] = ft_substr(lexer->line, lexer->start,
			lexer->i - lexer->start);
	return (0);
}

int	count_lexer_array(char *line)
{
	int	count;
	int	i;

	count = 3;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\"' || line[i] == '\'')
		{
			i++;
			while (line[i] && (line[i] == '\"' || line[i] == '\''))
				i++;
			count++;
			if (line[i] == '\0')
				return (count);
		}
		if (line[i] == ' ' || line[i] == '|'
			|| line[i] == '>' || line[i] == '<')
			count++;
		i++;
	}
	return (count);
}

int	init_lexer_struct(t_lexing *lexer, t_data *data, char *line)
{
	lexer->envp = data->envp;
	lexer->line = line;
	lexer->i = 0;
	lexer->start = 0;
	lexer->j = 0;
	lexer->exit_code = data->exit_code;
	lexer->res = malloc(sizeof(char *) * count_lexer_array(line) + 1);
	if (!lexer->res)
	{
		printf("Memory allocation failed\n");
		return (1);
	}
	return (0);
}
