/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_size.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/11 20:44:03 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/31 00:14:11 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static t_size	init_size(void)
{
	t_size	size;

	size.nlink = 0;
	size.pw_name = 0;
	size.gr_name = 0;
	size.st_size = 0;
	size.major = 0;
	size.minor = 0;
	size.device = 0;
	size.f_name = 0;
	size.w_cols = ft_getwincols();
	return (size);
}

static void		set_device(t_arg *list, t_size *size)
{
	int		tmp;

	tmp = major(list->info.st_rdev);
	if (tmp > size->major)
		size->major = tmp;
	tmp = minor(list->info.st_rdev);
	if (tmp > size->minor)
		size->minor = tmp;
	size->device = 1;
}

static void		set_digit(t_size *size)
{
	size->nlink = get_digits(size->nlink);
	size->major = get_digits(size->major);
	size->minor = get_digits(size->minor);
	size->st_size = get_digits(size->st_size);
	if (size->device)
		size->st_size = MAX(2 + size->major + size->minor, size->st_size);
}

t_size			set_size(t_arg *list)
{
	t_size			size;
	int				tmp;

	size = init_size();
	while (list)
	{
		if (list->info.st_nlink > size.nlink)
			size.nlink = list->info.st_nlink;
		tmp = ft_strlen(getpwuid(list->info.st_uid)->pw_name);
		if (tmp > size.pw_name)
			size.pw_name = tmp;
		tmp = ft_strlen(getgrgid(list->info.st_gid)->gr_name);
		if (tmp > size.gr_name)
			size.gr_name = tmp;
		if (list->info.st_size > size.st_size)
			size.st_size = list->info.st_size;
		tmp = ft_strlen(list->name);
		if (tmp > size.f_name)
			size.f_name = tmp;
		if (S_ISBLK(list->info.st_mode) || S_ISCHR(list->info.st_mode))
			set_device(list, &size);
		list = list->next;
	}
	set_digit(&size);
	return (size);
}

int				get_digits(int64_t n)
{
	int count;

	if (n)
		count = 0;
	else
		count = 1;
	while (n)
	{
		n /= 10;
		count++;
	}
	return (count);
}
