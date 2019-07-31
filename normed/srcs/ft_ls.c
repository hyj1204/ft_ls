/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/07 13:17:12 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/31 01:09:18 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int			ft_isdir(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

static void	recur(t_arg *obj_list, t_flag *flags)
{
	while (obj_list)
	{
		if (obj_list->is_dir && \
				ft_strcmp(obj_list->name, ".") \
				&& ft_strcmp(obj_list->name, ".."))
		{
			ft_putchar('\n');
			ft_ls(obj_list->path, flags);
		}
		obj_list = obj_list->next;
	}
}

static	int	dir_name(char *arg_name)
{
	int i;

	i = 0;
	while (arg_name[i] && (arg_name[i] == '.' || arg_name[i] == '/'))
	{
		i++;
	}
	return (i);
}

static int	check_is_file(t_arg **obj_list, \
char *arg_name, t_flag *flags, int sub)
{
	DIR	*dir;
	int	i;

	errno = 0;
	i = dir_name(arg_name);
	if (!(dir = opendir(arg_name)))
	{
		if (errno == 2)
			ft_printf("ls: %s: %s\n", arg_name, strerror(errno));
		else if (errno == 13 && ft_isdir(arg_name))
		{
			if (sub && flags->u_r)
				ft_printf("\n%s\nls: %s: %s", \
						arg_name, &arg_name[i], strerror(errno));
			else
				ft_printf("ls: %s: %s", &arg_name[i], strerror(errno));
		}
		else if (!(flags->u_r) && errno == 13)
			ft_printf("ls: %s: %s\n", arg_name, strerror(errno));
		else
			save_ftolist(obj_list, arg_name);
		return (1);
	}
	return (0);
}

int			ft_ls(char *arg_name, t_flag *flags)
{
	t_arg		*check_list;
	t_arg		*obj_list;
	int			total;
	t_size		size;
	static int	sub;

	total = 0;
	obj_list = NULL;
	if (check_is_file(&obj_list, arg_name, flags, sub))
		return (0);
	total = get_dir_list(&obj_list, arg_name, flags);
	sort_list(&obj_list, flags);
	if (sub && flags->u_r)
		(!obj_list) ? \
			ft_printf("\n%s:", arg_name) : ft_printf("\n%s:\n", arg_name);
	if (obj_list && flags->l)
		ft_printf("total %d\n", total);
	check_list = obj_list;
	size = set_size(obj_list);
	print_objs(check_list, size, flags);
	sub = 1;
	if (flags->u_r)
		recur(check_list, flags);
	free_list(&obj_list);
	return (0);
}
