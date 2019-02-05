/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsergien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 12:40:11 by tsergien          #+#    #+#             */
/*   Updated: 2018/10/23 12:40:24 by tsergien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm_corewar.h"

int			add(t_game *g, t_cursor *c)
{
	unsigned int	*args;
	unsigned char	*cod;
	int				offset;

	args = ft_memalloc(sizeof(unsigned int) * 3);
	offset = get_args(g->map, c, args, 3);
	cod = get_codage(g->map[(c->index + 1) % MEM_SIZE].byte);
	if (args_neok(args, cod, 3))
		return (offset + 2);
	if (cod[0] == REG_CODE && cod[1] == REG_CODE && cod[2] == REG_CODE)
	{
		c->registr[args[2]] = c->registr[args[0]] + c->registr[args[1]];
		c->carry = (!c->registr[args[2]]) ? 1 : 0;
	}
	free(args);
	free(cod);
	return (offset + 2);
}

int			sub(t_game *g, t_cursor *c)
{
	unsigned int	*args;
	unsigned char	*cod;
	int				offset;

	args = ft_memalloc(sizeof(unsigned int) * 3);
	offset = get_args(g->map, c, args, 3);
	cod = get_codage(g->map[(c->index + 1) % MEM_SIZE].byte);
	if (args_neok(args, cod, 3))
		return (offset + 2);
	if (cod[0] == REG_CODE && cod[1] == REG_CODE && cod[2] == REG_CODE)
	{
		c->registr[args[2]] = c->registr[args[0]] - c->registr[args[1]];
		c->carry = (!c->registr[args[2]]) ? 1 : 0;
	}
	free(args);
	free(cod);
	return (offset + 2);
}

/*
**  It will take an index and jump to this address
** if the carry is 1.
*/

int			zjmp(t_game *g, t_cursor *c)
{
	short int		t_dir;

	if (c->carry)
	{
		from_map(&t_dir, g->map, 2, c->index + 1);
		c->index = (c->index + t_dir % IDX_MOD) % MEM_SIZE;
	}
	return (3);
}

int			ldi(t_game *g, t_cursor *c)
{
	unsigned int	*args;
	unsigned char	*cod;
	int				offset;
	int				tmp;

	args = ft_memalloc(sizeof(unsigned int) * 3);
	offset = get_args(g->map, c, args, 3);
	cod = get_codage(g->map[(c->index + 1) % MEM_SIZE].byte & 252);
	if (args_neok(args, cod, 3))
		return (offset + 2);
	if (cod[0] != 0 && cod[1] != IND_CODE && cod[1] != 0 && cod[2] == REG_CODE)
	{
		tmp = (cod[0] == REG_CODE) ? c->registr[args[0]] : (short int)args[0];
		if (cod[0] == IND_CODE)
			from_map(&tmp, g->map, 4, tmp % IDX_MOD + c->index);
		tmp += (cod[1] == REG_CODE) ? c->registr[args[1]] : (short int)args[1];
		from_map(&c->registr[args[2]], g->map, 4, tmp % IDX_MOD + c->index);
	}
	free(args);
	free(cod);
	return (offset + 2);
}

int			sti(t_game *g, t_cursor *c)
{
	unsigned int	*args;
	unsigned char	*cod;
	int				offset;
	int				tmp;

	args = ft_memalloc(sizeof(unsigned int) * 3);
	offset = get_args(g->map, c, args, 3);
	cod = get_codage(g->map[(c->index + 1) % MEM_SIZE].byte & 252);
	if (args_neok(args, cod, 3))
		return (offset + 2);
	if (cod[0] == REG_CODE && cod[1] != 0 && cod[2] != IND_CODE && cod[2] != 0)
	{
		if (cod[1] == IND_CODE)
			from_map(&tmp, g->map, 4, args[1] % IDX_MOD);
		else if (cod[1] == REG_CODE)
			tmp = c->registr[args[1]];
		else
			tmp = (short int)args[1];
		tmp += (cod[2] == REG_CODE) ? c->registr[args[2]] : (short int)args[2];
		to_map(g, &c->registr[args[0]], c->index +
		tmp % IDX_MOD, c->parent_ind);
	}
	free(args);
	free(cod);
	return (offset + 2);
}
