/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsergien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 12:40:40 by tsergien          #+#    #+#             */
/*   Updated: 2018/10/23 12:40:41 by tsergien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm_corewar.h"

/*
** Take a random argument and a registry. Load the value of the first argument
** in the registry
*/

int			ld(t_game *g, t_cursor *c)
{
	unsigned int	*args;
	unsigned char	*cod;
	int				offset;

	args = (unsigned int *)malloc(sizeof(unsigned int) * 2);
	ft_bzero(args, sizeof(unsigned int) * 4);
	offset = get_args(g->map, c, args, 2);
	cod = get_codage(g->map[(c->index + 1) % MEM_SIZE].byte & 252);
	if (args_neok(args, cod, 2))
		return (offset + 2);
	if (cod[1] == REG_CODE && (cod[0] == DIR_CODE || cod[0] == IND_CODE))
	{
		if (cod[0] == IND_CODE)
			from_map(&(c->registr[args[1] % 17]), g->map, 4,
			c->index + (short int)args[0] % IDX_MOD);
		else if (cod[0] == DIR_CODE)
			c->registr[args[1] % 17] = args[0];
		c->carry = (c->registr[args[1] % 17] == 0) ? 1 : 0;
	}
	free(args);
	free(cod);
	return (offset + 2);
}

/*
**	args: T_REG,  T_REG | T_IND
** st: take a registry and a registry or an indirect and store the
** value of the registry toward a second argument.
*/

int			st(t_game *g, t_cursor *c)
{
	unsigned int	*args;
	unsigned char	*cod;
	int				offset;

	args = ft_memalloc(sizeof(unsigned int) * 2);
	offset = get_args(g->map, c, args, 2);
	cod = get_codage(g->map[(c->index + 1) % MEM_SIZE].byte & 252);
	if (args_neok(args, cod, 2))
		return (offset + 2);
	if (cod[0] == REG_CODE && cod[1] != 0 && cod[1] != DIR_CODE)
	{
		if (cod[1] == REG_CODE)
			c->registr[args[1] % 17] = c->registr[args[0] % 17];
		else if (cod[1] == IND_CODE)
			to_map(g, &c->registr[args[0]], c->index +
			(short int)args[1] % IDX_MOD, c->parent_ind);
	}
	free(args);
	free(cod);
	return (offset + 2);
}

int			and_(t_game *g, t_cursor *c)
{
	unsigned int	*args;
	unsigned char	*cod;
	int				offset;

	args = ft_memalloc(sizeof(unsigned int) * 3);
	offset = get_args(g->map, c, args, 3);
	cod = get_codage(g->map[(c->index + 1) % MEM_SIZE].byte);
	if (args_neok(args, cod, 3))
		return (offset + 2);
	args[0] = (cod[0] == REG_CODE) ? c->registr[args[0] % 17] : args[0];
	args[1] = (cod[1] == REG_CODE) ? c->registr[args[1] % 17] : args[1];
	args[0] = (cod[0] == IND_CODE) ? (unsigned short int)args[0] : args[0];
	args[1] = (cod[1] == IND_CODE) ? (unsigned short int)args[1] : args[1];
	if (cod[0] != 0 && cod[1] != 0 && cod[2] == 1)
	{
		c->registr[args[2] % 17] = args[0] & args[1];
		c->carry = (c->registr[args[2] % 17] == 0) ? 1 : 0;
	}
	free(cod);
	free(args);
	return (offset + 2);
}

int			or_(t_game *g, t_cursor *c)
{
	unsigned int	*args;
	unsigned char	*cod;
	int				offset;

	args = ft_memalloc(sizeof(unsigned int) * 3);
	offset = get_args(g->map, c, args, 3);
	cod = get_codage(g->map[(c->index + 1) % MEM_SIZE].byte);
	if (args_neok(args, cod, 3))
		return (offset + 2);
	args[0] = (cod[0] == REG_CODE) ? c->registr[args[0] % 17] : args[0];
	args[1] = (cod[1] == REG_CODE) ? c->registr[args[1] % 17] : args[1];
	args[0] = (cod[0] == IND_CODE) ? (unsigned short int)args[0] : args[0];
	args[1] = (cod[1] == IND_CODE) ? (unsigned short int)args[1] : args[1];
	if (cod[0] != 0 && cod[1] != 0 && cod[2] == 1)
	{
		c->registr[args[2] % 17] = args[0] | args[1];
		c->carry = (c->registr[args[2] % 17] == 0) ? 1 : 0;
	}
	free(cod);
	free(args);
	return (offset + 2);
}

int			xor_(t_game *g, t_cursor *c)
{
	unsigned int	*args;
	unsigned char	*cod;
	int				offset;

	args = ft_memalloc(sizeof(unsigned int) * 3);
	offset = get_args(g->map, c, args, 3);
	cod = get_codage(g->map[(c->index + 1) % MEM_SIZE].byte);
	if (args_neok(args, cod, 3))
		return (offset + 2);
	args[0] = (cod[0] == REG_CODE) ? c->registr[args[0] % 17] : args[0];
	args[1] = (cod[1] == REG_CODE) ? c->registr[args[1] % 17] : args[1];
	args[0] = (cod[0] == IND_CODE) ? (unsigned short int)args[0] : args[0];
	args[1] = (cod[1] == IND_CODE) ? (unsigned short int)args[1] : args[1];
	if (cod[0] != 0 && cod[1] != 0 && cod[2] == 1)
	{
		c->registr[args[2] % 17] = args[0] ^ args[1];
		c->carry = (c->registr[args[2] % 17] == 0) ? 1 : 0;
	}
	free(cod);
	free(args);
	return (offset + 2);
}
