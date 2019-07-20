/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 22:47:47 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/20 01:49:09 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

// static void	print_date(t_arg *a, t_flag *flags)
// {
// 	// if (!flags->u)
// 	// 	write(1, &ctime(&a->info.st_mtime)[4], 12);
// 	// else
// 	write(1, &ctime(&a->info.st_atime)[4], 12);
// 	ft_putchar(' ');
// }

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
	readlink(a->path, buf, 2047); //读取链接内容存在buf
	ft_printf(" -> %s", buf); //在文件名后面显示链接内容
}

void	print_permissions(t_arg *list)
{
	ssize_t xattr = 0;
	acl_t acl = NULL;
	ft_putchar((list->info.st_mode & S_IRUSR) ? 'r' : '-');
	ft_putchar((list->info.st_mode & S_IWUSR) ? 'w' : '-');
	ft_putchar((list->info.st_mode & S_IXUSR) ? 'x' : '-');
	ft_putchar((list->info.st_mode & S_IRGRP) ? 'r' : '-');
	ft_putchar((list->info.st_mode & S_IWGRP) ? 'w' : '-');
	ft_putchar((list->info.st_mode & S_IXGRP) ? 'x' : '-');
	ft_putchar((list->info.st_mode & S_IROTH) ? 'r' : '-');
	ft_putchar((list->info.st_mode & S_IWOTH) ? 'w' : '-');
	ft_putchar((list->info.st_mode & S_IXOTH) ? 'x' : '-');
	acl = acl_get_link_np(list->path, ACL_TYPE_EXTENDED);
	xattr = listxattr(list->path, NULL, 0, XATTR_NOFOLLOW);
	if (xattr < 0)
        xattr = 0;
    if (xattr > 0)
        ft_putchar('@'); //acl for extended attrible
    else if (acl != NULL) 
        ft_putchar('+');
    else
        ft_putchar(' ');

}

void		print_arg(t_arg *list, t_size size, t_flag *flags)
//显示每个对象的信息
{
	if (flags->l)
	{
		print_type(list); //显示第一部分的第一个字符，文件类型
		print_permissions(list);//显示权限字母
		ft_printf(" %*d ", size.nlink, list->info.st_nlink);

		ft_printf("%-*s  ", size.pw_name, getpwuid(list->info.st_uid)->pw_name);
		ft_printf("%-*s ", size.gr_name, getgrgid(list->info.st_gid)->gr_name);
		if (S_ISBLK(list->info.st_mode) || S_ISCHR(list->info.st_mode))
		//如果是设备文件（device file)的话
		{
			if (size.st_size > size.major + size.minor)
				ft_printf("%*s", size.st_size - size.major - size.minor - 2,
				"");
			ft_printf(" %*d, %*d ", size.major, major(list->info.st_rdev),
									size.minor, minor(list->info.st_rdev));
		}
		else
			ft_printf(" %*d ", size.st_size, list->info.st_size);
		// ft_printf("ctime = %s\n",ctime(&list->info.st_atime));
		write(1, &ctime(&list->info.st_mtime)[4], 12);
		//显示最近被修改的时间
		ft_putchar(' ');
		print_name(list, 0);
		(S_ISLNK(list->info.st_mode)) ? print_link(list) : 0;
	}
	else 
		print_name(list, size.f_name + 3);
	if (!list->next) //如果是显示的最后一个对象的话，就跳过。不显示回车。
		list->next = NULL;
	else if ((flags->one || flags->l) && list->next) 
		ft_putchar('\n');
	else
		ft_putchar(' ');
}