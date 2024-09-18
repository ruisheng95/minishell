/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_redisplay_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 23:12:42 by rng               #+#    #+#             */
/*   Updated: 2024/09/19 01:13:40 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

void rl_replace_line(const char *line, int len) 
{
    int old_point = rl_point;
    int old_end = rl_end;

    rl_point = 0;
    rl_end = 0;

    rl_free_line_state(); // instead of _rl_reset_line_state()

    rl_insert_text(line);
    rl_end = len;

    rl_point = old_point;
    rl_end = old_end;

    rl_on_new_line(); // instead of rl_refresh_line(0, 0)
    rl_redisplay();
}