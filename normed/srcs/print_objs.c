/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_objs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/11 21:01:44 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/31 00:49:30 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int		print_objs(t_arg *list, t_size size, t_flag *flags)
{
	int	checked;
	int col_num;

	checked = 0;
	col_num = get_line_capacity(size.f_name, size.w_cols);
	while (list)
	{
		if (!flags->one && !flags->l && \
				checked >= col_num && !(checked % col_num))
			ft_putchar('\n');
		print_list(list, size, flags);
		if ((flags->one || flags->l) && list->next)
			ft_putchar('\n');
		else if (!list->next)
			list->next = NULL;
		else
			ft_putchar(' ');
		list = list->next;
		checked++;
	}
	return (checked);
}

int		get_line_capacity(int name_len, int win_cols)
{
	int		n;

	n = 1;
	while ((name_len + 6) * n < win_cols - 2)
		n++;
	if (n - 2 > 1)
		return (n - 1);
	return (1);
}

void	free_list(t_arg **list)
{
	t_arg	*tmp;

	while (*list)
	{
		tmp = (*list)->next;
		free((*list)->name);
		free((*list)->path);
		free(*list);
		*list = tmp;
	}
}

void	print_permissions(t_arg *list)
{
	ft_putchar((list->info.st_mode & S_IRUSR) ? 'r' : '-');
	ft_putchar((list->info.st_mode & S_IWUSR) ? 'w' : '-');
	ft_putchar((list->info.st_mode & S_IXUSR) ? 'x' : '-');
	ft_putchar((list->info.st_mode & S_IRGRP) ? 'r' : '-');
	ft_putchar((list->info.st_mode & S_IWGRP) ? 'w' : '-');
	ft_putchar((list->info.st_mode & S_IXGRP) ? 'x' : '-');
	ft_putchar((list->info.st_mode & S_IROTH) ? 'r' : '-');
	ft_putchar((list->info.st_mode & S_IWOTH) ? 'w' : '-');
	ft_putchar((list->info.st_mode & S_IXOTH) ? 'x' : '-');
	if (listxattr(list->path, NULL, 0, XATTR_NOFOLLOW) > 0)
		ft_putchar('@');
	else if (acl_get_link_np(list->path, ACL_TYPE_EXTENDED) != NULL)
		ft_putchar('+');
	else
		ft_putchar(' ');
}
