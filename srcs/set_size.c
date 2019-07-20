/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_size.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/11 20:44:03 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/20 01:53:25 by yijhuang         ###   ########.fr       */
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
	size.w_cols = ft_getwincols();//获得当前终端窗口的列数
	return (size);
}

static void		set_device(t_arg *list, t_size *size)
{
	int		tmp;

	tmp = major(list->info.st_rdev);  //st_rdev：* device ID (if special file) */ 设备文件的设备号
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
	if (size->device)  //如果是设备文件时
		size->st_size = MAX(2 + size->major + size->minor, size->st_size);
}


t_size			set_size(t_arg *list)
//获得一个list里面对象各部分size的最大值
{
	t_size			size;
	int				tmp;

	size = init_size();
	while (list) //针对list里面的每一个节点（某个输入文件夹下的对象们）
	{
		if (list->info.st_nlink > size.nlink)
			size.nlink = list->info.st_nlink;
		//* number of hard links to the file */ 文件的硬连接数
		//如果输入里面的硬连接数多于初始值的话，就把list里面的值赋值给size
		tmp = ft_strlen(getpwuid(list->info.st_uid)->pw_name);
		//计算输入list里面的用户名的长度
		if (tmp > size.pw_name) //如果初始化的用户的长度小于计算的用户名时，把之前算出来的长长度给size
			size.pw_name = tmp;
		tmp = ft_strlen(getgrgid(list->info.st_gid)->gr_name);
		//计算group_name的长度
		if (tmp > size.gr_name) //把group_name的长度放入size结构体内
			size.gr_name = tmp;
		if (list->info.st_size > size.st_size)
			size.st_size = list->info.st_size;  //这个size是-l会显示的每个文件字节大小
		tmp = ft_strlen(list->name); //计算文件名的长度
		if (tmp > size.f_name)
			size.f_name = tmp; //设置文件名长度
		if (S_ISBLK(list->info.st_mode) || S_ISCHR(list->info.st_mode))
		//如果是device file的话（也就是block special file块设备文件或者character special file字符设备文件）
			set_device(list, &size); //获得响应的设备号device id
		list = list->next;
	}
	set_digit(&size);//然后把获得的size结构数据
	return (size);
}

int	ft_getwincols(void)
{
	struct winsize size;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	return (size.ws_col);
}

int		get_digits(int64_t n)
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
	//ft_printf("after count = %d\n", count);
	return (count);//返回位数，当输入是0的时候也会显示一位
}