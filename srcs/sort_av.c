/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_av.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 21:58:21 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/19 23:55:24 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static void	rev_av(int ac, char ***av, int begin)
{
	char	*tmp;

	while (begin < ac - 1)
	{
		tmp = (*av)[ac - 1]; //先tmp指向最后一个输入
		(*av)[ac - 1] = (*av)[begin]; //把第一个内容放到最后一个
		(*av)[begin] = tmp;//再把原来最后一个放到第一个
		ac--;
		begin++;
	}
    //第一个和最后一个一直调换内容，然后两端各收缩一位。
}

void		sort_av(int ac, char ***av, int begin, t_flag *flags)
//***av指的就是输入字符串的地址
{
    int		end;
	int		i;
	char	*tmp;

    end = ac - 1;
	while (end > begin)
	{
		i = begin;
		while (i < end)
		{
			if (ft_strcmp((*av)[i], (*av)[i + 1]) > 0)
            //按字母顺序判断，如果前一个字符串大于后一个字符串的话，说明后面的字符串应该在前面
			{
				tmp = (*av)[i];
				(*av)[i] = (*av)[i + 1]; //调换两个字符串的内容
				(*av)[i + 1] = tmp;
            }
            i++;
		}
        end--;
	}
    // 例如：4321 先按，43，42，41，把4（最大值）移到最后。然后在看3214中的前三个，把3移到最后。
    // 以此类推。只要end位置大于begin位置的话，就可以一直比较。
	if (flags->r)
		rev_av(ac, av, begin);
}