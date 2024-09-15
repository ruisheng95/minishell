/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:56:22 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/15 15:02:14 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**lexer(char *line, t_data *data)
{
	t_lexing	lexer;

	if (!line || line[0] == '\0')
		return (NULL);
	if (init_lexer_struct(&lexer, data, line) == 1)
		return (NULL);
	while (lexer.line[lexer.i])
	{
		while (lexer.line[lexer.i] == ' ')
			lexer.i++;
		lexer.start = lexer.i;
		if (lexer.line[lexer.i] == '|' || lexer.line[lexer.i] == '>'
			|| lexer.line[lexer.i] == '<')
		{
			lexer_helper2(&lexer);
		}
		else
		{
			if (lexer_helper3(&lexer) == -1)
				return (NULL);
		}
		lexer.j++;
	}
	lexer.res[lexer.j] = NULL;
	return (lexer.res);
}

t_tokens	*init_token_list(t_data *data)
{
	t_tokens	*list;
	t_tokens	*last_node;
	t_tokens	*newnode;
	int			i;

	if (!data || !data->tokens)
		return (NULL);
	i = -1;
	list = NULL;
	while (data->tokens[++i])
	{
		if (ft_strcmp(data->tokens[i], "$EMPTY") != 0)
		{
			newnode = malloc(sizeof(t_tokens));
			newnode->token = data->tokens[i];
			newnode->next = NULL;
			newnode->prev = last_node;
			if (!list)
				list = newnode;
			else
				last_node->next = newnode;
			last_node = newnode;
		}
	}
	return (list);
}

void	identify_tokens_list(t_tokens *tokens)
{
	while (tokens)
	{
		if (tokens->token[0] == '|')
			tokens->type = Pipe;
		else if (tokens->token[0] == '>')
		{
			if (tokens->token[1] == '>')
				tokens->type = redir_out_append;
			else
				tokens->type = redir_out_overwrite;
		}
		else if (tokens->token[0] == '<')
		{
			if (tokens->token[1] == '<')
				tokens->type = heredoc;
			else
				tokens->type = redir_input;
		}
		else
			tokens->type = string;
		tokens = tokens->next;
	}
}

int	identify_tokens_list_2_helper_cuz_norm(t_tokens *list)
{
	if (list->type == string && list->prev
		&& (list->prev->type == Pipe || list->prev->type == infile
			|| list->prev->type == outfile_append
			|| list->prev->type == outfile_overwrite
			|| list->prev->type == heredoc_lim))
		return (command);
	else if (list->type == string && list->prev
		&& list->prev->type == redir_input)
		return (infile);
	else if (list->type == string && list->prev
		&& list->prev->type == redir_out_overwrite)
		return (outfile_overwrite);
	else if (list->type == string && list->prev
		&& list->prev->type == redir_out_append)
		return (outfile_append);
	else if (list->type == string && list->prev
		&& list->prev->type == heredoc)
		return (heredoc_lim);
	else if (list->type == string)
		return (arguments);
	return (0);
}

void	identify_tokens_list2(t_tokens *list)
{
	t_tokens	*first_token;
	t_tokens	*last_token;

	first_token = list;
	last_token = get_last_node(list);
	if (!last_token)
		return ;
	while (list)
	{
		if (list == first_token && list->type == string)
			list->type = command;
		else if (list->type == string)
		{
			list->type = identify_tokens_list_2_helper_cuz_norm(list);
		}
		list = list->next;
	}
}

// void	print_tokens_list(t_tokens *list)
// {
// 	int	i;

// 	i = 0;
// 	while (list != NULL)
// 	{
// 		printf("token %d : %s, type : %d\n", i, list->token, list->type);
// 		list = list->next;
// 		i++;
// 	}
// }

// void	print_token_array(char **str)
// {
// 	int	i;

// 	i = 0;
// 	if (!str)
// 	{
// 		printf("ERORRR LOLLL\n");
// 		return ;
// 	}
// 	while (str[i])
// 	{
// 		printf("token %d = %s\n", i, str[i]);
// 		i++;
// 	}
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	t_tokens *list;
// 	char *str = "$EMPTY echo hi";
// 	t_data data;
// 	data.envp = envp;
// 	if((data.tokens = lexer(str, &data)) == NULL)
// 		exit(1);
// 	print_token_array(data.tokens);
// 	printf("-----------------------------------\n");
// 	list = init_token_list(&data);
// 	identify_tokens_list(list);
// 	expansion(list, &data);
// 	tokens_list_processing(list);
// 	printf("-----------------------------------\n");
// 	remove_quotes_from_token_list(list);
// 	printf("-----------------------------------\n");
// 	identify_tokens_list2(list);
// 	if(check_valid_list(list) == 1)
// 		exit(1);
// 	print_tokens_list(list);
// }

// int main(int argc, char **argv, char **envp)
// {
// 	char *str = "export b=\"ho hi\"";
// 	char **lex = semi_ft_split(str);
// 	print_token_array(lex);
// }