# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tsergien <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/10/19 10:23:10 by tsergien          #+#    #+#              #
#    Updated: 2018/10/26 14:51:36 by tsergien         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all:
	@make -C VM
	@make -C asm

clean:
	@make clean -C VM
	@make clean -C asm

fclean:
	@make fclean -C VM
	@make fclean -C asm

re: fclean all
