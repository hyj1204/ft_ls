/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 17:36:10 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/22 21:16:54 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int	ft_isdir(const char *path) //检查这个路径的对象是不是一个文件夹
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

static int	print_file(int ac, char **av, t_flag *flags)
{
	int		i;
	int		f_amount;  //代表输入字符串里面有多少个非文件夹
	t_arg	*list;
	t_size	size;

    // ft_printf("after get list arg_index = %d\n", arg_index);
	i = arg_index;
	f_amount = 0;
	list = NULL;
	while (i < ac)
	{
		if (!ft_isdir(av[i]))
		{
			save_ftolist(&list, av[i]); 
			f_amount++;
		}
		i++;
	}
	sort_list(&list, flags);
	size = set_size(list); //此时list里只有所有输入字符串的非文件夹对象,获得这些对象显示size
	print_objs(list, size, flags); //把所有在list里面的文件显示出来，这是一个共用函数
	free_list(&list);
	return (f_amount);
}

static void	print_dir(int ac, char **av, t_flag *flags)
{
	char	*path;

	while (arg_index < ac)
	{
		if (ft_isdir(av[arg_index])) //如果输入字符串是文件夹的时候就显示。这个函数之前已经把非文件夹都处理掉了。
		{
			// ft_printf("dir = %s\n", av[arg_index]);
			// ft_printf("f_amount = %d\n", f_amount);
			if ((f_amount > 0)&& (flags->l || flags->one)) 
				f_amount = -1;//把这个值设为-1，代表flags->l || flags->one情况下，第一个文件夹不需要另起一行
			else if ((f_amount == -1 || f_amount > 0) && !flags->l) 
				ft_putchar('\n');//如果之前的print_normal已经有显示非文件夹对象的话，就显示回车，另起一行。
			ft_putstr(av[arg_index]);
			ft_putendl(":");
			path = fix_path(av[arg_index], NULL);
			// ft_printf("path = %s\n",path);
			ft_ls(path, flags);
			free(path);
		}
		// if (arg_index + 1 == ac && !flags->R && !flags->l && !flags->one) //当正常显示多个文件夹的时候，最后一个文件夹里面的对象显示完之后要回车。
		// 	ft_putchar('\n');
		arg_index++;
	}
}

static void	print_a_dir(char **av, t_flag *flags)
{
	ft_ls(av[arg_index], flags);

}

int main(int ac, char **av)
{
    t_flag flags;

    arg_index = 1; //设置全局变量arg_index=1，即从第一个输入参数开始判断
    flags = getflags(ac, av); //获得flag结构,arg_index会走到-flags后面一个参数位置
    // ft_printf("after get list arg_index = %d\n", arg_index);
    if (arg_index == ac) //如果此时已经走过最后一个参数（-flag是最后一个参数），则说明是当前文件夹操作
        ft_ls(".", &flags);
	if (arg_index + 1 == ac)
	{
		print_a_dir(av, &flags);
		if (flags.R || flags.l || flags.one)
			ft_putchar('\n'); 
	}
	else
	{
		sort_av(ac, &av, arg_index, &flags);//把输入字符串排序，直接修改*av数列里的内容(所以输入是av数列的地址，也就是修改这个地址上的数据)
    	f_amount = print_file(ac, av, &flags);
    	print_dir(ac, av, &flags);
		if (flags.R || flags.l || flags.one)
			ft_putchar('\n');    
	}
}