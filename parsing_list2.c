/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_list2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 23:29:51 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/16 00:27:08 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redir_input_token(t_node *newnode, t_tokens **token)
{
	newnode->type = REDIR_INPUT;
	if ((*token)->next->type == INFILE)
	{
		newnode->redir_in.in_file = (*token)->next->token;
		newnode->redir_in.fd = open(newnode->redir_in.in_file, O_RDONLY);
		if (newnode->redir_in.fd == -1)
		{
			error_str(2, newnode->redir_in.in_file);
			return (-1);
		}
		*token = (*token)->next->next;
	}
	else
	{
		free(newnode);
		return (-1);
	}
	return (0);
}

int	handle_redir_out_append_token(t_node *newnode, t_tokens **token)
{
	newnode->type = REDIR_OUT_APPEND;
	if ((*token)->next->type == OUTFILE_APPEND)
	{
		newnode->redir_out.outfile = (*token)->next->token;
		newnode->redir_out.fd = open(newnode->redir_out.outfile,
				O_WRONLY | O_CREAT | O_APPEND, 0777);
		*token = (*token)->next->next;
	}
	else
	{
		free(newnode);
		return (-1);
	}
	return (0);
}

int	handle_redir_out_overwrite_token(t_node *newnode, t_tokens **token)
{
	newnode->type = REDIR_OUT_OVERWRITE;
	if ((*token)->next->type == OUTFILE_OVERWRITE)
	{
		newnode->redir_out.outfile = (*token)->next->token;
		newnode->redir_out.fd = open(newnode->redir_out.outfile,
				O_WRONLY | O_TRUNC | O_CREAT, 0777);
		*token = (*token)->next->next;
	}
	else
	{
		free(newnode);
		return (-1);
	}
	return (0);
}

int	handle_heredoc_token(t_node *newnode, t_tokens **token)
{
	newnode->type = HEREDOC;
	if ((*token)->next->type == HEREDOC_LIM)
	{
		newnode->heredoc_obj.limiter = (*token)->next->token;
		*token = (*token)->next->next;
	}
	else
	{
		free(newnode);
		return (-1);
	}
	return (0);
}

void	handle_command_token(t_node *newnode, t_tokens **token)
{
	int	i;

	i = 0;
	newnode->type = S_COMMAND;
	if ((*token)->token != NULL && (*token)->token[0] != '\0')
	{
		newnode->simple_cmd.cmd = ft_strdup((*token)->token);
		newnode->simple_cmd.array = ft_calloc(sizeof(char *), 1000);
		newnode->simple_cmd.array[i++] = ft_strdup((*token)->token);
		*token = (*token)->next;
		while (*token != NULL && (*token)->type == ARGUMENTS)
		{
			newnode->simple_cmd.array[i++] = ft_strdup((*token)->token);
			*token = (*token)->next;
		}
		newnode->simple_cmd.array[i] = NULL;
	}
	else
	{
		newnode->simple_cmd.cmd = ft_strdup("");
		newnode->simple_cmd.array = malloc(sizeof(char *) * 2);
		newnode->simple_cmd.array[0] = ft_strdup("");
		newnode->simple_cmd.array[1] = NULL;
		*token = (*token)->next;
	}
}
