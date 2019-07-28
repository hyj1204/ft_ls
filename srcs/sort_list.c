/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 22:57:29 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/28 04:45:00 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

void	sort_list(t_arg **obj_list, t_flag *flags)
{
	// ft_printf("before sort list\n");
	// print_list(*obj_list);
	merge_sort(obj_list, flags);
	// ft_printf("after sort list\n");
	// print_list(*obj_list);
	if (flags->r)
		reverse_list(obj_list);
}

void	reverse_list(t_arg **obj_list)
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

void			merge_sort(t_arg **list, t_flag *flags)
{
	t_arg	*head;
	t_arg	*a;
	t_arg	*b;

	head = *list;
	if (!head || !head->next)  //如果文件夹内（list里没有元素）
		return ;
	split(head, &a, &b);
	//list放入，得到a指向list的头地址，b指向list最后一个节点
	merge_sort(&a, flags);
	// ft_printf("marge a list\n");
	// print_list(a);
	merge_sort(&b, flags);
	// ft_printf("marge b list\n");
	// print_list(b);
	*list = lexical_sorted(a, b, flags);
	// ft_printf("print list\n");
	// print_list(*list);

}

void		split(t_arg *head, t_arg **a, t_arg **b)
//list放入，得到a指向list的头地址，b指向list最后一个节点
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

t_arg	*lexical_sorted(t_arg *a, t_arg *b, t_flag *flags)
//找出链表中显示最靠前的结构体
{
	t_arg	*result;

	result = NULL;
	if (!a)
		return (b);
	if (!b)
		return (a);
	if ((flags->t ? (b->info.st_mtime - a->info.st_mtime) : ft_strcmp(a->name, b->name)) < 0)
	//比较两个结构体的最后修改时间，正数的时候
	//如果有tflag的话，就用timecmp函数比较两个结构.小于0时，a是最近被修改的。大于0时b是最近被修改的。
	//没有tflag的话，就用ft_strcmp函数比较两个结构.小于0时，a名字靠前。大于0时b名字靠前。
	//判断两个结构体，返回显示靠前的那个结构体
	{
		result = a;
		result->next = lexical_sorted(a->next, b, flags);
	}
	else
	{
		result = b;
		result->next = lexical_sorted(a, b->next, flags);
	}
	return (result);//返回显示靠前的那一个结构
}
