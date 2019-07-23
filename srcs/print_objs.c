/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_objs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/11 21:01:44 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/22 20:00:36 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

void		print_objs(t_arg *list, t_size size, t_flag *flags)
{
	int	checked;
	int col_num;

	checked = 0; //用于检查已经print了多少个对象
	// ft_printf("size-w_cols = %d\n", size.w_cols);
	// ft_printf("size-f_name = %d\n", size.f_name);
	col_num = get_line_capacity(size.f_name, size.w_cols);
	//获得一行显示多少个对象
	// printf("col_num = %d\n", col_num);
	while (list) //当list指针当前所指的node存在的时候
	{
		if (!flags->one && !flags->l && checked >= col_num && !(checked % col_num)) 
		//当没有l-flag时,例如一行需显示4个，当检测5个的时候，checked还是4，此时需要回车一次
			ft_putchar('\n');
		// print_list(list);
		// ft_printf("one time\n");
		print_arg(list, size, flags);
		if (!flags->R && !flags->l && !flags->one && !list->next) //当正常显示多个文件夹的时候，最后一个文件夹里面的对象显示完之后要回车。
			ft_putchar('\n');
		list = list->next; //会把指针一直移到list后一个node
		checked++;
	}
}

int		get_line_capacity(int name_len, int win_cols)
{
	int		n;

	n = 1;
	while ((name_len + 2) * n < win_cols - 5) //根据窗口显示效果测出这个方法
		n++;
	if (n - 2 > 1)
		return (n - 2);
	return (1);
}