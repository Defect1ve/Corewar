/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm_get_champ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsergien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 15:40:04 by tsergien          #+#    #+#             */
/*   Updated: 2018/10/18 15:42:27 by tsergien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "../includes/vm_corewar.h"

static void	get_other_flags(t_game *g, char **av, int *i)
{
	if (!ft_strcmp(av[*i], "-a"))
		g->flags.a = 1;
	else if (!ft_strcmp(av[*i], "-l"))
		g->flags.leaks = 1;
	else if (!ft_strcmp(av[*i], "-v"))
		g->flags.v = 1;
	else if (!ft_strcmp(av[*i], "-dump"))
		g->flags.nbr_cycles = ft_atoi(av[++(*i)]);
	else if (!ft_strcmp(av[*i], "-color"))
		g->flags.dump_color = 1;
}

static void	get_flags(t_game *g, int ac, char **av)
{
	int				i;
	int				ind;

	i = 0;
	ind = 0;
	while (++i < ac)
	{
		if (!ft_strcmp(av[i], "-a") || !ft_strcmp(av[i], "-l")
		|| !ft_strcmp(av[i], "-v") || !ft_strcmp(av[i], "-dump")
		|| !ft_strcmp(av[i], "-color"))
			get_other_flags(g, av, &i);
		else if (!ft_strcmp(av[i], "-n"))
			get_player(g, &i, av);
		else
		{
			while (ind < 4 && g->champ[ind].filled)
				++ind;
			get_champ(g, open(av[i], O_RDONLY), ind);
			g->flags.n++;
		}
	}
	if (g->flags.n != g->champs_num)
		ft_error("Invalid players order\n");
}

static void	init(t_game *g, int ac, char **av)
{
	int		i;

	i = -1;
	ft_bzero(g, sizeof(t_game));
	g->champs_num = count_players(ac, av);
	g->pause = -1;
	g->cycle = 0;
	g->cycles_limit = 50;
	g->flags.nbr_cycles = -1;
	g->cycles_to_die = CYCLE_TO_DIE;
	if (g->champs_num < 1 || g->champs_num > 4)
		usage();
	get_flags(g, ac, av);
	while (++i < g->champs_num)
		add_cursor(i, g);
}

void		text_mode(t_game *g)
{
	int			i;

	i = -1;
	g->pause = 0;
	ft_printf("Introducing contestants...\n");
	while (++i < g->champs_num)
		ft_printf("* Player %d, weighing %d bytes, \"%s\" (\"%s\") !\n",
	i + 1, g->champ[i].prog_size, g->champ[i].prog_name,
	g->champ[i].comment);
	while (g->cursor)
		do_step(g);
	ft_printf("Contestant %d, \"%s\", has won !\n",
		g->winner_ind + 1, g->champ[g->winner_ind].prog_name);
}

int			main(int argc, char **argv)
{
	t_game		*g;
	int			i;

	g = (t_game *)malloc(sizeof(t_game));
	init(g, argc, argv);
	i = -1;
	if (g->flags.v)
		show_field(g);
	else
		text_mode(g);
	if (g->flags.leaks && !g->flags.v)
		system("leaks -quiet corewar");
	return (0);
}
