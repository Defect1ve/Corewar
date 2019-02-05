/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsergien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 12:40:06 by tsergien          #+#    #+#             */
/*   Updated: 2018/11/09 12:59:30 by ikotvits         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm_corewar.h"

void		to_map(t_game *g, void *src, unsigned ind, int player)
{
	unsigned int	i;
	unsigned char	*csrc;

	i = -1;
	csrc = (unsigned char *)src;
	while (++i < 4)
	{
		g->map[(ind + i) % MEM_SIZE].byte = csrc[3 - i];
		g->map[(ind + i) % MEM_SIZE].champ = player + 1;
		g->map[(ind + i) % MEM_SIZE].rewrited = g->cycle;
	}
}

void		from_map(void *dst, t_field *map, size_t size, unsigned ind)
{
	unsigned int	i;
	unsigned char	*cdst;

	i = -1;
	cdst = (unsigned char *)dst;
	while (++i < size)
		cdst[size - i - 1] = map[(ind + i) % MEM_SIZE].byte;
}

char		args_neok(unsigned int *args, unsigned char *cod, int arg_num)
{
	int	i;

	i = -1;
	while (++i < arg_num)
	{
		if (cod[i] == REG_CODE && (args[i] == 0
		|| args[i] > 16))
		{
			free(args);
			free(cod);
			return (1);
		}
	}
	return (0);
}

int			get_args(t_field *m, t_cursor *c, unsigned int *args,
			unsigned int args_num)
{
	unsigned int	i;
	unsigned int	offset;
	unsigned int	c_ind;
	unsigned char	*cod;

	i = -1;
	offset = 0;
	cod = get_codage(m[(c->index + 1) % MEM_SIZE].byte);
	c_ind = (c->index + 1 + g_tab[c->command - 1].codage) % MEM_SIZE;
	while (++i < args_num)
	{
		(cod[i] == REG_CODE) ? from_map(&args[i], m, 1, c_ind + offset++) : 0;
		if (cod[i] == DIR_CODE)
		{
			from_map(&args[i], m, g_tab[c->command - 1].lab, c_ind + offset);
			offset += g_tab[c->command - 1].lab;
		}
		else if (cod[i] == IND_CODE)
		{
			from_map(&args[i], m, 4, c_ind + (offset += 2) - 2);
			args[i] = args[i] >> 16;
		}
	}
	free(cod);
	return (offset);
}

int			live(t_game *g, t_cursor *c)
{
	char			name;
	unsigned int	arg;
	int				l;

	c->last_live = g->cycle;
	name = g->map[(c->index + 4) % MEM_SIZE].byte;
	from_map(&arg, g->map, 4, c->index + 1);
	g->nbr_live++;
	l = -1;
	while (++l < g->champs_num)
		if (!(l + arg + 1))
		{
			if (!g->flags.v && !g->flags.nbr_cycles)
				ft_printf("A process shows that player %d %s is alive\n",
			l + 1, g->champ[l].prog_name);
			g->champ[l].current_lives++;
			g->map[c->index].alive_champ = l;
			g->champ[l].last_live = g->cycle;
			g->map[c->index].alive = g->cycle;
			g->winner_ind = l;
		}
	return (5);
}
