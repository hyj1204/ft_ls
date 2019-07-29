/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_objs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/11 21:01:44 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/29 03:27:39 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int		print_objs(t_arg *list, t_size size, t_flag *flags)
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
		print_list(list, size, flags);
		//下面是显示list显示完内容之后的换行控制
		if ((flags->one || flags->l) && list->next)
			ft_putchar('\n');
		else if (!list->next) //如果是显示的最后一个对象的话，就跳过。不显示回车。
			list->next = NULL;
		else
			ft_putchar(' ');
		list = list->next; //会把指针一直移到list后一个node
		checked++;
	}
	return checked;
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