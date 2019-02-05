/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visual2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsergien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/15 11:26:08 by tsergien          #+#    #+#             */
/*   Updated: 2018/11/15 11:26:09 by tsergien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm_corewar.h"
#include <fcntl.h>

static void		controller2(int c, t_game *g)
{
	if (c == 32)
	{
		if (g->cycle)
			(g->pause) ? system("kill -CONT $(pgrep 'afplay') &> /dev/null") :
			system("kill -STOP $(pgrep 'afplay') &> /dev/null");
		g->pause = (!g->pause) ? 1 : 0;
	}
	if (c == 113 && g->cycles_limit > 1)
		g->cycles_limit = g->cycles_limit - 10 < 1 ? 1 : g->cycles_limit - 10;
	else if (c == 114 && g->cycles_limit < 1000)
		g->cycles_limit = (g->cycles_limit + 10 > 1000) ?
		1000 : g->cycles_limit + 10;
	else if (c == 119 && g->cycles_limit > 1)
		g->cycles_limit--;
	else if (c == 101 && g->cycles_limit < 1000)
		g->cycles_limit += 1;
	else if (c == 3 || c == 27)
	{
		system("killall afplay");
		endwin();
		exit(0);
	}
	else if (c == 's' && g->cursor)
		do_step(g);
}

void			controller(WINDOW **win1, WINDOW **win2, int c, t_game *g)
{
	if (c == 410)
	{
		delwin(*win1);
		delwin(*win2);
		clear();
		refresh();
		*win1 = newwin(68, 197, 0, 0);
		*win2 = newwin(68, 58, 0, 196);
	}
	else
	{
		if (!g->pause && !(clock() % 50) && system("pgrep afplay > /dev/null")
		&& g->cursor)
			system("afplay src/music.mp3 &");
		controller2(c, g);
	}
}

void			init_pairs(void)
{
	init_color(COLOR_MAGENTA, 410, 410, 410);
	init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(7, COLOR_BLACK, COLOR_BLACK);
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(101, COLOR_WHITE, COLOR_GREEN);
	init_pair(102, COLOR_WHITE, COLOR_BLUE);
	init_pair(103, COLOR_WHITE, COLOR_RED);
	init_pair(104, COLOR_WHITE, COLOR_CYAN);
	init_pair(10, COLOR_BLACK, COLOR_GREEN);
	init_pair(20, COLOR_BLACK, COLOR_BLUE);
	init_pair(30, COLOR_BLACK, COLOR_RED);
	init_pair(40, COLOR_BLACK, COLOR_CYAN);
	init_pair(50, COLOR_BLACK, COLOR_BLACK);
}

t_cursor		*remove_cursor(t_game *g, t_cursor *del)
{
	t_cursor	*tmp;

	tmp = g->cursor;
	g->champ[del->parent_ind % 4].processes--;
	if (g->cursor == del)
	{
		g->cursor = tmp->next;
		free(tmp);
		return (g->cursor);
	}
	while (tmp && tmp->next)
	{
		if (tmp->next == del)
		{
			tmp->next = tmp->next->next;
			free(del);
			return (tmp->next);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void			get_player(t_game *g, int *i, char **av)
{
	int	n;

	n = ft_atoi(av[++(*i)]) - 1;
	(n < 0 || n > g->champs_num - 1) ? ft_error("Invalid index\n") : 0;
	g->champ[n].filled ? ft_error("Player is already assigned!\n") : 0;
	g->flags.n++;
	get_champ(g, open(av[++(*i)], O_RDONLY), n);
}
