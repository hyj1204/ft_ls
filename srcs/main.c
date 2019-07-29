/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 17:36:10 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/29 03:23:44 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static int	print_file(int ac, char **av, t_flag *flags)
{
	int		i;
	int		f_amount;  //代表输入字符串里面有多少个非文件夹
	t_arg	*list;
	t_size	size;
	
	i = arg_index;
	f_amount = 0;
	list = NULL;
	while (i < ac)
	{
		errno = 0;
		if (!ft_isdir(av[i]))
		{
			save_ftolist(&list, av[i]); 
			if (errno != 2) //当输入名字存在的时候，算是会显示出来的文件数
				f_amount++;
		}
		i++;
	}
	sort_list(&list, flags);
	size = set_size(list); //此时list里只有所有输入字符串的非文件夹对象,获得这些对象显示size
	i = print_objs(list, size, flags); //把所有在list里面的文件显示出来，这是一个共用函数
	if ( i > 1)  //测试时，当只有一个输入文件的时候，会自动换行。但是多于一个文件的时候就没有换行。所以这个加上。
		ft_printf("\n");
	free_list(&list);
	return (f_amount);
}

static void	print_dir(int ac, char **av, t_flag *flags)
{
	char	*path;
	static int first_dir; //0代表第一个dir，1代表后面的

	while (arg_index < ac)
	{
		if (ft_isdir(av[arg_index])) //如果输入字符串是文件夹的时候就显示。这个函数之前已经把非文件夹都处理掉了。
		{
			// ft_printf("dir = %s\n", av[arg_index]);
			// ft_printf("f_amount = %d\n", f_amount);
			if (f_amount && !first_dir) //是第一个文件夹的时候
			{
				 ft_printf("\n\n");
				 f_amount = 0;
				 first_dir = 1;
			}
			else if (first_dir == 1)
				ft_printf("\n");
			ft_putstr(av[arg_index]);
			ft_putendl(":");
			path = fix_path(av[arg_index], NULL);
			// ft_printf("path = %s\n",path);
			ft_ls(path, flags);
			if (arg_index + 1 < ac) //多个文件夹输入的时候，显示完对象回车+空行
				ft_printf("\n");
			if (arg_index + 1 == ac)
				ft_printf("\n");
			free(path);
		}
		arg_index++;
	}
}

int main(int ac, char **av)
{
    t_flag flags;

    arg_index = 1; //设置全局变量arg_index：代表目前是第几个参数。从第一个输入参数开始判断。
    flags = getflags(ac, av); //获得flag结构,arg_index会走到-flags后面一个参数位置
    if (arg_index == ac) //如果此时已经走过最后一个参数（-flag是最后一个参数），则说明是当前文件夹操作
	{
        ft_ls(".", &flags);
		ft_putchar('\n'); 
	}
	else if (arg_index + 1 == ac) //只有一个输入参数的时候,要么显示这个文件，要么显示这个文件夹下的对象
	{
		print_file(ac, av, &flags);
		if (errno != 2)
		{
			ft_ls(av[arg_index], &flags);
			ft_putchar('\n');
		} 
	}
	else
	{
		//多个输入的时候，先把所有输入先排序。直接修改*av数列里的内容(所以输入是av数列的地址，也就是修改这个地址上的数据)
		sort_av(ac, &av, arg_index, &flags);
    	f_amount = print_file(ac, av, &flags);
    	print_dir(ac, av, &flags);
		if ((flags.R || flags.l || flags.one)&& (arg_index + 1 < ac))
			ft_putchar('\n');
	}
	return 0;
}