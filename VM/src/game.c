/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsergien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 13:07:31 by tsergien          #+#    #+#             */
/*   Updated: 2018/10/22 13:07:34 by tsergien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm_corewar.h"

void			add_cursor(int champ_ind, t_game *g)
{
	t_cursor	*new_elem;
	int			pos;
	static int	id = 0;

	pos = champ_ind * (MEM_SIZE / g->champs_num);
	new_elem = (t_cursor *)malloc(sizeof(t_cursor));
	ft_bzero(new_elem, sizeof(t_cursor));
	new_elem->index = pos;
	new_elem->id = ++id;
	new_elem->parent_ind = champ_ind;
	new_elem->last_live = g->cycle;
	new_elem->command = g->map[pos].byte;
	new_elem->cycles_to_exec = g_tab[g->map[pos].byte - 1].cycles;
	new_elem->registr[1] = -(champ_ind + 1);
	new_elem->next = g->cursor;
	g->champ[(int)new_elem->parent_ind].processes++;
	g->cursor = new_elem;
}

void			exec_comm(t_cursor *c, t_game *g,
int (*f)(t_game *g, t_cursor *c))
{
	unsigned int		shift;

	if (!c->command)
	{
		c->command = g->map[c->index].byte;
		c->cycles_to_exec = (c->command) ? g_tab[c->command - 1].cycles : 0;
	}
	if (c->command == 0 || c->command > 16)
	{
		c->index = (c->index + 1) % MEM_SIZE;
		c->command = 0;
	}
	else if (c->cycles_to_exec == 1)
	{
		shift = f(g, c);
		if (c->command != 9 || !c->carry)
			c->index = (shift + c->index) % MEM_SIZE;
		c->command = 0;
	}
	else
		c->cycles_to_exec--;
}

/*
** • If during one of those checkup we notice that there has been at least
** one NBR_LIVE
** execution of live since the latest check up, we will decrease CYCLE_TO_DIE of
** CYCLE_DELTA units.
*/

void			check_alives(t_game *g)
{
	t_cursor		*c;
	int				champ_ind;
	static int		checks;

	c = g->cursor;
	while (c)
		c = (g->cycle - c->last_live >= g->cycles_to_die) ?
		remove_cursor(g, c) : c->next;
	champ_ind = -1;
	while (++champ_ind < g->champs_num)
		g->champ[champ_ind].current_lives = 0;
	if (g->nbr_live >= NBR_LIVE || checks == MAX_CHECKS)
	{
		g->cycles_to_die = (g->cycles_to_die >= CYCLE_DELTA) ?
		g->cycles_to_die - CYCLE_DELTA : 0;
		checks = 0;
	}
	else
		checks++;
	g->nbr_live = 0;
}

void			do_step(t_game *g)
{
	t_cursor				*tmp;
	static int				(*f[16])(t_game *g, t_cursor *c) = {&live, &ld, &st,
	&add, &sub, &and_, &or_, &xor_, &zjmp, &ldi, &sti, &fork_,
	&lld, &lldi, &lfork, &aff};

	tmp = g->cursor;
	if (g->cycle == (unsigned int)g->flags.nbr_cycles)
		dump_map(g);
	g->cycle++;
	while (tmp)
	{
		if (tmp->command < 1 || tmp->command > 16)
			exec_comm(tmp, g, NULL);
		else
			exec_comm(tmp, g, f[tmp->command - 1]);
		tmp = tmp->next;
	}
	if (g->cycle - g->last_check == g->cycles_to_die || !g->cycles_to_die)
	{
		check_alives(g);
		g->last_check = g->cycle;
	}
}

/*
** Every CYCLE_TO_DIE cycles, the machine needs to make sure that each process
** has executed at least one live since the last check. A process that does
**not abide by this rule will be killed immediately with a virtual foamy bat
** (bonus for sound effect!)
**
** • The game is over when all processes are dead.
*/
