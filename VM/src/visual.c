/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visual.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsergien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 16:26:38 by tsergien          #+#    #+#             */
/*   Updated: 2018/10/19 16:26:41 by tsergien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm_corewar.h"

static void		print_map(t_game *g, WINDOW *win)
{
	int		i;
	int		c;

	i = -1;
	while (++i < MEM_SIZE)
	{
		if (!g->map[i].champ ||
		((g->cycle - g->map[i].rewrited < 49) && g->map[i].rewrited) ||
		(g->map[i].alive && g->cycle - g->map[i].alive < 49))
			wattron(win, A_BOLD);
		if (g->map[i].alive && g->cycle - g->map[i].alive < 49)
			c = 100 + g->map[i].alive_champ + 1;
		else if (g->map[i].champ || (g->map[i].rewrited &&
		(g->cycle - g->map[i].rewrited < 49)))
			c = g->map[i].champ;
		else
			c = 7;
		wattron(win, COLOR_PAIR(c));
		mvwprintw(win, i / 64 + 2, 3 * (i % 64) + 3, "%02x", g->map[i].byte);
		wattroff(win, COLOR_PAIR(c));
		if (!g->map[i].champ ||
		(g->map[i].rewrited && (g->cycle - g->map[i].rewrited < 49)) ||
		(g->map[i].alive && g->cycle - g->map[i].alive < 49))
			wattroff(win, A_BOLD);
	}
}

static void		cursors_output(t_game *g, WINDOW *win)
{
	t_cursor	*t;
	int			c;

	t = g->cursor;
	while (t)
	{
		if (g->map[t->index].alive &&
		g->cycle - g->map[t->index].alive < 49)
		{
			t = t->next;
			continue ;
		}
		c = g->map[t->index].champ ? 10 * g->map[t->index].champ : 5;
		wattron(win, COLOR_PAIR(c));
		if (g->map[t->index].rewrited
		&& g->cycle - g->map[t->index].rewrited < 49)
			wattron(win, A_BOLD);
		mvwprintw(win, t->index / 64 + 2, 3 * (t->index % 64) + 3,
		"%02x", g->map[t->index].byte);
		if (g->map[t->index].rewrited &&
		g->cycle - g->map[t->index].rewrited < 49)
			wattroff(win, A_BOLD);
		t = t->next;
		wattroff(win, COLOR_PAIR(c));
	}
}

static void		show_panels(WINDOW **win1, WINDOW **win2, t_game *g)
{
	controller(win1, win2, wgetch(*win2), g);
	if (!g->cursor)
	{
		wattron(*win2, A_BOLD);
		mvwprintw(*win2, 27 + g->champs_num * 4, 2, "Player ");
		wattron(*win2, COLOR_PAIR(g->winner_ind + 1));
		mvwprintw(*win2, 27 + g->champs_num * 4, 9, "%d %s", g->winner_ind + 1,
		g->champ[g->winner_ind].prog_name);
		wattroff(*win2, COLOR_PAIR(g->winner_ind + 1));
		mvwprintw(*win2, 27 + g->champs_num * 4, 11 +
		ft_strlen(g->champ[g->winner_ind].prog_name), " won!");
		mvwprintw(*win2, 28 + g->champs_num * 4 + 1,
		2, "Press escape to exit ");
		wattroff(*win2, A_BOLD);
	}
	if ((g->cycle + 1 - g->cycles_to_die == g->last_check) && g->cycle != 1)
		diagram1(*win2, g, 14 + g->champs_num * 4);
	(!g->pause && g->cursor) ? do_step(g) : 0;
	print_map(g, *win1);
	print_panel(g, *win2);
	cursors_output(g, *win1);
}

static void		do_while(WINDOW **win1, WINDOW **win2, t_game *g)
{
	clock_t		prev;

	prev = 0;
	init_pairs();
	while (1)
	{
		while (clock() - prev <= 1000000 / (unsigned long)g->cycles_limit)
			continue ;
		prev = clock();
		nodelay(*win2, true);
		wattron(*win1, COLOR_PAIR(5));
		wattron(*win2, COLOR_PAIR(5));
		wborder(*win1, 42, 42, 42, 42, 42, 42, 42, 42);
		wborder(*win2, 42, 42, 42, 42, 42, 42, 42, 42);
		refresh();
		wrefresh(*win1);
		wrefresh(*win2);
		wattroff(*win1, COLOR_PAIR(5));
		wattroff(*win2, COLOR_PAIR(5));
		show_panels(win1, win2, g);
	}
}

void			show_field(t_game *g)
{
	WINDOW	*win1;
	WINDOW	*win2;

	if (g->flags.nbr_cycles)
	{
		while (g->cursor && g->flags.nbr_cycles != -1
		&& g->cycle < (unsigned int)g->flags.nbr_cycles)
			do_step(g);
	}
	initscr();
	noecho();
	curs_set(0);
	win1 = newwin(68, 197, 0, 0);
	win2 = newwin(68, 58, 0, 196);
	keypad(win1, TRUE);
	keypad(win2, TRUE);
	start_color();
	raw();
	do_while(&win1, &win2, g);
}
