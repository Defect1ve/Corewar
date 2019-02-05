/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm_corewar.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsergien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 15:36:45 by tsergien          #+#    #+#             */
/*   Updated: 2018/10/18 15:39:36 by tsergien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_COREWAR_H
# define VM_COREWAR_H
# include "../../libft/libft.h"
# include "op.h"
#include <time.h>
# include <ncurses.h>

void			ft_error(char *str);
void			get_champ(t_game *g, int fd, int ind);
void			show_field(t_game *g);
void			usage(void);
char			is_alives(t_cursor *c);
void			add_cursor(int champ_ind, t_game *g);
void			from_map(void *dst, t_field *map, size_t size, unsigned ind);
int				count_players(int ac, char **av);
void			get_player(t_game *g, int *i, char **av);
void			to_map(t_game *g, void *src, unsigned ind, int player);
unsigned char	*get_codage(unsigned char codage);
int				get_args(t_field *map, t_cursor *c, unsigned int *args,
				unsigned int args_num);
void			do_step(t_game *g);
char			args_neok(unsigned int *args, unsigned char *cod, int arg_num);
void			dump_map(t_game *g);
t_cursor		*remove_cursor(t_game *g, t_cursor *del);

/*
** VISUAL
*/

void			print_panel(t_game *g, WINDOW *win);
void			diagram1(WINDOW *win, t_game *g, int pos);
void			init_pairs(void);
void			controller(WINDOW **win1, WINDOW **win2, int c, t_game *g);

/*
** FUNCTIONS
*/

int				live(t_game *g, t_cursor *c);
int				ld(t_game *g, t_cursor *c);
int				st(t_game *g, t_cursor *c);
int				add(t_game *g, t_cursor *c);
int				sub(t_game *g, t_cursor *c);
int				and_(t_game *g, t_cursor *c);
int				or_(t_game *g, t_cursor *c);
int				xor_(t_game *g, t_cursor *c);
int				zjmp(t_game *g, t_cursor *c);
int				ldi(t_game *g, t_cursor *c);
int				sti(t_game *g, t_cursor *c);
int				fork_(t_game *g, t_cursor *c);
int				lld(t_game *g, t_cursor *c);
int				lldi(t_game *g, t_cursor *c);
int				lfork(t_game *g, t_cursor *c);
int				aff(t_game *g, t_cursor *c);

#endif
