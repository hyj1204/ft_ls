/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 22:57:29 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/31 01:09:45 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int			timecmp(t_arg *a, t_arg *b, t_flag *flags)
{
	if (flags->u)
		return (b->info.st_atime - a->info.st_atime);
	return (b->info.st_mtime - a->info.st_mtime);
}

void		sort_list(t_arg **obj_list, t_flag *flags)
{
	merge_sort(obj_list, flags);
	if (flags->r)
		reverse_list(obj_list);
}

void		merge_sort(t_arg **list, t_flag *flags)
{
	t_arg	*head;
	t_arg	*a;
	t_arg	*b;

	head = *list;
	if (!head || !head->next)
		return ;
	split(head, &a, &b);
	merge_sort(&a, flags);
	merge_sort(&b, flags);
	*list = lexical_sorted(a, b, flags);
}

void		split(t_arg *head, t_arg **a, t_arg **b)
{
	t_arg	*slow;
	t_arg	*fast;

	slow = head;
	fast = head->next;
	while (fast)
	{
		fast = fast->next;
		if (fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*a = head;
	*b = slow->next;
	slow->next = NULL;
	return ;
}

void		reverse_list(t_arg **obj_list)
{
	t_arg	*prev;
	t_arg	*current;
	t_arg	*next;

	prev = NULL;
	current = *obj_list;
	while (current)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	*obj_list = prev;
}
