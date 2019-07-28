# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/12/13 23:24:03 by yijhuang          #+#    #+#              #
#    Updated: 2019/07/27 03:27:15 by yijhuang         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls

FILES = srcs/main.c \
		srcs/get_flags.c \
		srcs/ft_ls.c \
		srcs/get_list.c \
		srcs/sort_list.c \
		srcs/set_size.c \
		srcs/print_objs.c \
		srcs/print_list.c \
		srcs/sort_av.c \

INC = -I includes \
	  -I libft/include \

OBJ = $(FILES:.c=.o)

all: $(NAME)

$(NAME):$(FILES)
	make -C libft/
	gcc -Wall -Werror -Wextra $(FILES) $(INC) -L libft/ -lft -o $(NAME)

clean:
	make -C libft/ clean
	rm -f $(OBJS)

fclean: clean
	make -C libft/ fclean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all re
