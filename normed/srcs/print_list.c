/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 22:47:47 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/31 00:11:44 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static void	print_date(t_arg *a, t_flag *flags)
{
	if (flags->u)
		write(1, &ctime(&a->info.st_atime)[4], 12);
	else
		write(1, &ctime(&a->info.st_mtime)[4], 12);
	ft_putchar(' ');
}

static void	print_type(t_arg *list)
{
	if (S_ISREG(list->info.st_mode))
		ft_putchar('-');
	else if (S_ISDIR(list->info.st_mode))
		ft_putchar('d');
	else if (S_ISBLK(list->info.st_mode))
		ft_putchar('b');
	else if (S_ISCHR(list->info.st_mode))
		ft_putchar('c');
	else if (S_ISFIFO(list->info.st_mode))
		ft_putchar('p');
	else if (S_ISLNK(list->info.st_mode))
		ft_putchar('l');
	else if (S_ISSOCK(list->info.st_mode))
		ft_putchar('s');
	else
		ft_putchar('?');
}

static void	print_name(t_arg *list, int size)
{
	if (size)
		ft_printf("%-*s ", size, list->name);
	else
		ft_putstr(list->name);
}

static void	print_link(t_arg *a)
{
	char	buf[2048];

	ft_bzero(buf, 2048);
	readlink(a->path, buf, 2047);
	ft_printf(" -> %s", buf);
}

void		print_list(t_arg *list, t_size size, t_flag *flags)
{
	if (flags->l)
	{
		print_type(list);
		print_permissions(list);
		ft_printf(" %*d ", size.nlink, list->info.st_nlink);
		ft_printf("%-*s  ", size.pw_name, getpwuid(list->info.st_uid)->pw_name);
		ft_printf("%-*s ", size.gr_name, getgrgid(list->info.st_gid)->gr_name);
		if (S_ISBLK(list->info.st_mode) || S_ISCHR(list->info.st_mode))
		{
			if (size.st_size > size.major + size.minor)
				ft_printf("%*s", size.st_size - size.major - size.minor - 2,
				"");
			ft_printf(" %*d, %*d ", size.major, major(list->info.st_rdev),
									size.minor, minor(list->info.st_rdev));
		}
		else
			ft_printf(" %*d ", size.st_size, list->info.st_size);
		print_date(list, flags);
		print_name(list, 0);
		(S_ISLNK(list->info.st_mode)) ? print_link(list) : 0;
	}
	else
		print_name(list, size.f_name + 5);
}
