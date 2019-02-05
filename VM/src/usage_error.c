/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usage_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsergien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/21 14:52:10 by tsergien          #+#    #+#             */
/*   Updated: 2018/10/21 14:54:19 by tsergien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm_corewar.h"

unsigned char	*get_codage(unsigned char codage)
{
	unsigned char	*cod;

	cod = (unsigned char *)malloc(sizeof(unsigned char) * 3);
	cod[0] = (codage & 192) >> 6;
	cod[1] = (codage & 48) >> 4;
	cod[2] = (codage & 12) >> 2;
	return (cod);
}

void			ft_error(char *str)
{
	write(2, str, ft_strlen(str) + 1);
	exit(0);
}

void			usage(void)
{
	ft_printf("\x1b[38;5;46musage:\033[0m \x1b[38;5;117m./corewar[-v]\
	[-dump nbr_cycles] [-n player_number]\
	<player1.cor> <...>\n\033[0m");
	ft_printf("\x1b[38;5;218m");
	ft_printf("        -a            : Do not print output from \"aff\"\n\
	-dump N       : Dumps memory after N cycles then exits\n\
	-n N          : set player\'s number to N\n\
	-v            : visualisation\n\
	-l            : leaks\n\
	-color        : colors in dump\n\
	-v -dump N    : start visualisation of game after N cycles\n\033[0m");
	exit(0);
}

int				count_players(int ac, char **av)
{
	int		i;
	int		players;

	i = 0;
	players = 0;
	while (++i < ac)
	{
		if (ft_strstr(av[i], ".cor"))
			players++;
	}
	return (players);
}

/*
** GREEN = 46
** BLUE = 117
** RED = 196
** CYAN = 87
** PINK 219
** RESET_COLOR = \033[0m
*/

void			dump_map(t_game *g)
{
	int i;
	int	color;

	i = -1;
	if (g->flags.v)
		return ;
	ft_printf("0x");
	while (++i < MEM_SIZE)
	{
		if (!(i % 64))
			ft_printf("%#0.4x : ", i);
		color = !g->map[i].champ ? 250 : 196;
		color = (g->map[i].champ == 2) ? 46 : color;
		color = (g->map[i].champ == 3) ? 219 : color;
		color = (g->map[i].champ == 4) ? 117 : color;
		if (g->flags.dump_color)
			ft_printf("\x1b[38;5;%dm", color);
		ft_printf("% 02x ", g->map[i].byte);
		if (g->flags.dump_color)
			ft_printf("\033[0m");
		if (!((i + 1) % 64) && i)
			ft_printf("\n");
	}
	exit(0);
}
