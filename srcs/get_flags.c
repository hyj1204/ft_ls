/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_flags.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 21:23:12 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/29 03:22:06 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

t_flag	init_flags(void)
{
	t_flag		flags;

	flags.a = 0;
	flags.l = 0;
	flags.R = 0;
	flags.r = 0;
	flags.t = 0;
	flags.u = 0;
	flags.one = 0;
	return (flags);
}

char get_opt(int ac, char **av, char *flags_str)
//经过这个函数之后全局变量arg_index会走到有-flag参数的后面一个位置。
//返回值是所遇到的flags的机器码（一个）
{
	static int	nextchar; 
	//静态int 默认从0开始,每一次根据上一次的结果来计算
	//因为这个函数一遇到flag字符就会return那个字符的机器码，所以当遇到有多个flags时（-lt这种)，就需要下次从t位置开始，所以要用静态函数
	int			i;

	if (arg_index >= ac || av[arg_index][0] != '-')
	//如果当前的全局变量已经走到ac（即已经走过最后一个参数）或者 当前参数的开头不是‘-’的时候。
	//就返回-1直接退出这个函数，意味着当前走到的参数不是一个可以取到flag的参数（是文件名或者文件夹名）
	//针对 ls -l -R 多个flag的情况，所以要检查每一个输入av
		return (-1);
	i = 0;
	nextchar++; //nextchar第一次从1开始，因为第一个字符是-，所以就跳过了.
	while (i < (int)ft_strlen(flags_str))
	{
		if (av[arg_index][nextchar] == flags_str[i]) //如果第一个参数的第二个值等于flags_str中任意一个字符的话
		{
			if (!(av[arg_index][nextchar + 1])) //先检查下这是不是flags的最后一个字符，是的话
			{
				arg_index++; //就看下一个参数，下一个参数就是文件夹的名字（或者指向空白，指的是当前文件夹）
				nextchar = 0;
			}
			return (flags_str[i]); //返回获得的flags的char, 例如：-R返回的就是字符R。一次返回一个字符。
		}
		i++;
	}
	not_flag_char = av[arg_index][nextchar]; //是-开头的已存在的参数，但是-后面不是flag字符而是个普通字符,则用全局变量保存这个字符，用于之后显示报错信息函数用。
	return (0);  
}

void		err_print(void) 
{
	ft_printf("ls: illegal option -- %c\n", not_flag_char);
	ft_printf("usage: ls [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]\n");
	exit(1);
}

t_flag	set_flags(char c, t_flag flags)
{
	if (c == 'a')
		flags.a = 1;
	else if (c == 'l')
		flags.l = 1;
	else if (c == 'R')
		flags.R = 1;
	else if (c == 'r')
		flags.r = 1;
	else if (c == 't')
		flags.t = 1;
	else if (c == '1')
		flags.one = 1;
	else if (c == 'u')
		flags.u = 1;
	return (flags);
}

t_flag			getflags(int ac, char **av)
{
	t_flag flags;
	char flags_c;

	flags = init_flags(); //初始化flags结构
	while ((flags_c = get_opt(ac, av, "alRrt1u")) != -1)
	//flags_c是getopt的返回值，这个返回值获得的flag字符。每次getopt读出来有值的情况
	//getopt函数会以此读取每个一个输入av字符串，读完一个之后会让全局变量arg_index++，使得while可以继续判断下一个参数是不是flag
	{
		if (flags_c == 0) //如果是-开头参数，但是里面的字符不是flag字符，只是普通字符，则显示报错信息
			err_print(); 
		flags = set_flags(flags_c, flags);//就把获得的这个char赋值到flags结构里
	}
	return flags;
}