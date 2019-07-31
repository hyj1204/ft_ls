/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 17:36:10 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/31 01:09:52 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static int		print_file(int ac, char **av, t_flag *flags)
{
	int			i;
	int			g_amount;
	t_arg		*list;
	t_size		size;

	i = g_index;
	g_amount = 0;
	list = NULL;
	while (i < ac)
	{
		errno = 0;
		if (!ft_isdir(av[i]))
		{
			save_ftolist(&list, av[i]);
			(errno != 2) ? g_amount++ : 0;
		}
		i++;
	}
	sort_list(&list, flags);
	size = set_size(list);
	i = print_objs(list, size, flags);
	if (i > 1)
		ft_printf("\n");
	free_list(&list);
	return (g_amount);
}

static	void	print_dir(int ac, char **av, t_flag *flags)
{
	char		*path;
	static	int	first_dir;

	while (g_index < ac)
	{
		if (ft_isdir(av[g_index]))
		{
			if (g_amount && !first_dir)
			{
				ft_printf("\n\n");
				g_amount = 0;
			}
			first_dir = 1;
			if (first_dir == 1)
				ft_printf("\n");
			ft_printf("%s:\n", av[g_index]);
			path = fix_path(av[g_index], NULL);
			ft_ls(path, flags);
			if (g_index + 1 <= ac)
				ft_printf("\n");
			free(path);
		}
		g_index++;
	}
}

int				main(int ac, char **av)
{
	t_flag		flags;

	flags = getflags(ac, av);
	if (g_index == ac)
	{
		ft_ls(".", &flags);
		ft_putchar('\n');
	}
	else if (g_index + 1 == ac)
	{
		print_file(ac, av, &flags);
		if (errno != 2 && !(ft_ls(av[g_index], &flags)))
			ft_putchar('\n');
	}
	else
	{
		sort_av(ac, &av, g_index, &flags);
		g_amount = print_file(ac, av, &flags);
		print_dir(ac, av, &flags);
		if ((flags.u_r || flags.l || flags.one) && (g_index + 1 < ac))
			ft_putchar('\n');
	}
	return (0);
}
