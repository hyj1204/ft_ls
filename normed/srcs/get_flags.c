/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_flags.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 21:23:12 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/31 01:09:05 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

t_flag		init_flags(void)
{
	t_flag		flags;

	flags.a = 0;
	flags.l = 0;
	flags.u_r = 0;
	flags.r = 0;
	flags.t = 0;
	flags.u = 0;
	flags.one = 0;
	return (flags);
}

char		get_opt(int ac, char **av, char *flags_str)
{
	static int	nextchar;
	int			i;

	if (g_index >= ac || av[g_index][0] != '-')
		return (-1);
	i = 0;
	nextchar++;
	while (i < (int)ft_strlen(flags_str))
	{
		if (av[g_index][nextchar] == flags_str[i])
		{
			if (!(av[g_index][nextchar + 1]))
			{
				g_index++;
				nextchar = 0;
			}
			return (flags_str[i]);
		}
		i++;
	}
	g_nflag_char = av[g_index][nextchar];
	return (0);
}

void		err_print(void)
{
	ft_printf("ls: illegal option -- %c\n", g_nflag_char);
	ft_printf("usage: ls [Ralrtu1] [file ...]\n");
	exit(1);
}

t_flag		set_flags(char c, t_flag flags)
{
	if (c == 'a')
		flags.a = 1;
	else if (c == 'l')
		flags.l = 1;
	else if (c == 'R')
		flags.u_r = 1;
	else if (c == 'r')
		flags.r = 1;
	else if (c == 't')
		flags.t = 1;
	else if (c == '1')
		flags.one = 1;
	else if (c == 'u')
		flags.u = 1;
	return (flags);
}

t_flag		getflags(int ac, char **av)
{
	t_flag	flags;
	char	flags_c;

	g_index = 1;
	flags = init_flags();
	while ((flags_c = get_opt(ac, av, "alRrt1u")) != -1)
	{
		if (flags_c == 0)
			err_print();
		flags = set_flags(flags_c, flags);
	}
	return (flags);
}
