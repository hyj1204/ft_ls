/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/07 13:17:12 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/20 01:36:00 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

// void print_list(t_arg *head) {
//     t_arg *current_node;
//     current_node = head;
//    	while (current_node) 
//     { 
//         ft_printf("list obj = %s\n", current_node->name);
//         current_node = current_node->next;
//     }
// } //for test

static void		recur(t_arg *obj_list, t_flag *flags)
{
	while (obj_list)
	{
		if (obj_list->is_dir && ft_strcmp(obj_list->name, ".") && ft_strcmp(obj_list->name, ".."))
        //如果对象是一个文件夹的时候，同时不是隐藏文件的时候（跟隐藏文件比较，值不为0）
		{
			ft_putchar('\n'); //先正常显示文件夹名字后回车
            // ft_printf("path:%s\n", obj_list->path);
			ft_ls(obj_list->path, flags); //再把文件路径放入ls函数中
            // ft_printf("2return\n");
		}
		obj_list = obj_list->next;
	}
}

void		free_list(t_arg **list)
{
	t_arg	*tmp;

	while (*list)
	{
		tmp = (*list)->next;
		free((*list)->name);
		free((*list)->path);
		free(*list);
		*list = tmp;
	}
}


//输入文件夹名和flags
void ft_ls(char *arg_name, t_flag *flags)
{
    t_arg	*check_list;
    t_arg   *obj_list;
	int     total;
    t_size  size;
    int size1;
    int size2;
    static int sub;

    size1 = 10;
    size2 = 8;
    obj_list = NULL;
    total = get_list(&obj_list, arg_name, flags);//把所有的输入参数（名字）对象，建立一个链表
    // print_list(obj_list);
	sort_list(&obj_list, flags);
    // ft_printf("after_sort\n");
    // print_list(obj_list);s
    if (sub && flags->R)//当R在子文件夹的状态下
    {
		// ft_printf("%s:\n", &arg_name[ft_strncmp(arg_name, "././", 4) ? 0 : 2]);
        ft_printf("%s:\n", arg_name);
    }
    if (total && obj_list && flags->l)
        ft_printf("total %d\n", total);
    check_list = obj_list;
	size = set_size(obj_list); //给t_size结构赋值，获得各个需要显示对象的长度len,用于-l时显示
    // ft_printf("w_cols = %d\n", size.w_cols);

    print_objs(check_list, size, flags);
    if (total == 0 && errno) //如果之前读取这个文件夹时产生错误信息的情况下
    {
        while (*arg_name == '.' || *arg_name == '/')
            arg_name++;
        ft_printf("ls: %s: ", arg_name);
        perror(NULL); 
    }
    sub = 1;
	if (flags->R)
    {
        (!(size.f_name))? 0:ft_putchar('\n'); //如果文件夹里面没有文件名字的话，就不需要回车
        recur(obj_list, flags);
    }
	free_list(&obj_list);
}


