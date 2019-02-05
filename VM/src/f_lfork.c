/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsergien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 12:40:35 by tsergien          #+#    #+#             */
/*   Updated: 2018/11/13 13:24:55 by tsergien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm_corewar.h"

int				fork_(t_game *g, t_cursor *c)
{
	short int			t_dir;
	t_cursor			*temp;
	int					i;

	i = 0;
	from_map(&t_dir, g->map, 2, c->index + 1);
	add_cursor(c->parent_ind, g);
	temp = g->cursor;
	temp->carry = c->carry;
	temp->last_live = c->last_live;
	temp->parent_ind = c->parent_ind;
	ft_bzero(temp->registr, sizeof(unsigned int) * 17);
	while (++i < REG_NUMBER + 1)
		temp->registr[i] = c->registr[i];
	temp->index = (c->index + t_dir % IDX_MOD) % MEM_SIZE;
	temp->command = (g->map[temp->index].byte && g->map[temp->index].byte < 17)
	? g->map[temp->index].byte : 0;
	temp->cycles_to_exec = (temp->command != 0) ?
	g_tab[temp->command - 1].cycles : 0;
	return (3);
}

int				lld(t_game *g, t_cursor *c)
{
	unsigned int	*args;
	unsigned char	*cod;
	int				offset;

	args = ft_memalloc(sizeof(unsigned int) * 2);
	offset = get_args(g->map, c, args, 2);
	cod = get_codage(g->map[(c->index + 1) % MEM_SIZE].byte & 252);
	if (args_neok(args, cod, 2))
		return (offset + 2);
	if (cod[1] == REG_CODE && (cod[0] == DIR_CODE || cod[0] == IND_CODE))
	{
		if (cod[0] == IND_CODE)
			from_map(&c->registr[args[1]], g->map, 4,
			c->index + (short int)args[0]);
		else if (cod[0] == DIR_CODE)
			c->registr[args[1]] = args[0];
		c->carry = (c->registr[args[1]] == 0) ? 1 : 0;
	}
	free(args);
	free(cod);
	return (offset + 2);
}

int				lldi(t_game *g, t_cursor *c)
{
	unsigned int	*args;
	unsigned char	*cod;
	int				offset;

	args = ft_memalloc(sizeof(unsigned int) * 3);
	offset = get_args(g->map, c, args, 3);
	cod = get_codage(g->map[(c->index + 1) % MEM_SIZE].byte);
	if (args_neok(args, cod, 3))
		return (offset + 2);
	if (cod[0] != 0 && cod[1] != IND_CODE && cod[1] != 0 && cod[2] == REG_CODE)
	{
		args[0] = (cod[0] == REG_CODE) ? c->registr[args[0]]
		: (unsigned short int)args[0];
		if (cod[0] == IND_CODE)
			from_map(&args[0], g->map, 4, args[0] % IDX_MOD + c->index);
		args[1] = (cod[1] == REG_CODE) ? c->registr[args[1]]
		: (unsigned short int)args[1];
		from_map(&c->registr[args[2]], g->map, 4, args[0] +
		args[1] + c->index);
	}
	free(args);
	free(cod);
	return (offset + 2);
}

int				lfork(t_game *g, t_cursor *c)
{
	short int			t_dir;
	t_cursor			*temp;
	int					i;

	i = 0;
	from_map(&t_dir, g->map, 2, c->index + 1);
	add_cursor(c->parent_ind, g);
	temp = g->cursor;
	temp->carry = c->carry;
	temp->last_live = c->last_live;
	temp->parent_ind = c->parent_ind;
	ft_bzero(temp->registr, sizeof(unsigned int) * 17);
	while (++i < REG_NUMBER + 1)
		temp->registr[i] = c->registr[i];
	temp->index = (c->index + t_dir) % MEM_SIZE;
	temp->command = (g->map[temp->index].byte && g->map[temp->index].byte < 17)
	? g->map[temp->index].byte : 0;
	temp->cycles_to_exec = (temp->command != 0) ?
	g_tab[(temp->command - 1)].cycles : 0;
	return (3);
}

int				aff(t_game *g, t_cursor *c)
{
	unsigned int	*args;
	unsigned char	*cod;
	int				offset;

	args = ft_memalloc(sizeof(unsigned int) * 1);
	offset = get_args(g->map, c, args, 1);
	cod = get_codage(g->map[(c->index + 1) % MEM_SIZE].byte & 252);
	if (args_neok(args, cod, 1))
		return (offset + 2);
	if (cod[0] == REG_CODE && !g->flags.v && !g->flags.a)
		ft_printf("%c", c->registr[args[0]] % 256);
	free(args);
	free(cod);
	return (offset + 2);
}
