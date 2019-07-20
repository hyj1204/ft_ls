/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 17:36:10 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/19 23:57:39 by yijhuang         ###   ########.fr       */
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
	// print_list(list);
	sort_list(&list, flags);
	size = set_size(list); //此时list里只有所有输入字符串的非文件夹对象,获得这些对象显示size
	print_objs(list, size, flags); //把所有在list里面的文件显示出来，这是一个共用函数
	ft_putchar('\n');
	free_list(&list);
	return (f_amount);
}

static void	print_dir(int ac, char **av, t_flag *flags, int f_amount)
{
	char	*path;
	// int		m;

	// m = arg_index + 1 < ac ? 1 : 0;
	while (arg_index < ac)
	{
		if (ft_isdir(av[arg_index])) //如果输入字符串是文件夹的时候就显示。这个函数之前已经把非文件夹都处理掉了。
		{
			// ft_printf("f_amount = %d\n", f_amount);
			f_amount ? ft_putchar('\n') : 0; //如果之前的print_normal已经有显示非文件夹对象的话，就显示回车，另起一行。
			f_amount = 1; //同时把这个值设为1，也就是第一个文件夹里面的内容显示完之后，遇到新的文件夹都会另起一行。
			ft_putstr(av[arg_index]);
			ft_putendl(":");
			// m ? ft_putstr(av[arg_index]) : 0;
			// m ? ft_putendl(":") : 0;
			path = fix_path(av[arg_index], NULL);
			// ft_printf("path = %s\n",path);
			ft_ls(path, flags);
			// (flags->l || flags->one)? 0 : ft_putchar('\n');
			free(path);
		}
		arg_index++;
	}
}

int main(int ac, char **av)
{
    t_flag flags;
    int check;
    
    arg_index = 1; //设置全局变量arg_index=1，即从第一个输入参数开始判断
    flags = getflags(ac, av); //获得flag结构,arg_index会走到-flags后面一个参数位置
    // ft_printf("after get list arg_index = %d\n", arg_index);
    if (arg_index == ac) //如果此时已经走过最后一个参数（-flag是最后一个参数），则说明是当前文件夹操作
        ft_ls(".", &flags);
	sort_av(ac, &av, arg_index, &flags);//把输入字符串排序，直接修改*av数列里的内容(所以输入是av数列的地址，也就是修改这个地址上的数据)
    check = print_file(ac, av, &flags);
    print_dir(ac, av, &flags, check);
    
    // while (arg_index < ac) //把每一个输入字符串都放入ft_ls函数
    // { 
    //     ft_ls(av[arg_index], &flags);
    //     arg_index++;
    // }
    //此时arg_index应该是在最后一个参数的后面一个位置
    // ft_printf("after ft_ls arg_index = %d\n", arg_index);
    
    
}