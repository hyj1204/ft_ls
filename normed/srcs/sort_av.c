/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_av.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 21:58:21 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/31 00:54:54 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static void	rev_av(int ac, char ***av, int begin)
{
	char	*tmp;

	while (begin < ac - 1)
	{
		tmp = (*av)[ac - 1];
		(*av)[ac - 1] = (*av)[begin];
		(*av)[begin] = tmp;
		ac--;
		begin++;
	}
}

void		sort_av(int ac, char ***av, int begin, t_flag *flags)
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
			{
				tmp = (*av)[i];
				(*av)[i] = (*av)[i + 1];
				(*av)[i + 1] = tmp;
			}
			i++;
		}
		end--;
	}
	if (flags->r)
		rev_av(ac, av, begin);
}

int			ft_getwincols(void)
{
	struct winsize size;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	return (size.ws_col);
}

t_arg		*lexical_sorted(t_arg *a, t_arg *b, t_flag *flags)
{
	t_arg	*result;

	result = NULL;
	if (!a)
		return (b);
	if (!b)
		return (a);
	if (flags->t)
	{
		if (timecmp(a, b, flags) < 0)
		{
			result = a;
			result->next = lexical_sorted(a->next, b, flags);
		}
		else
		{
			result = b;
			result->next = lexical_sorted(a, b->next, flags);
		}
	}
	else
		result = lexical_sorted2(a, b);
	return (result);
}

t_arg		*lexical_sorted2(t_arg *a, t_arg *b)
{
	t_arg	*result;

	result = NULL;
	if (!a)
		return (b);
	if (!b)
		return (a);
	if (ft_strcmp(a->name, b->name) < 0)
	{
		result = a;
		result->next = lexical_sorted2(a->next, b);
	}
	else
	{
		result = b;
		result->next = lexical_sorted2(a, b->next);
	}
	return (result);
}
