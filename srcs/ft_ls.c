/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/07 13:17:12 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/22 20:07:48 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

void print_list(t_arg *head) {
    t_arg *current_node;
    current_node = head;
   	while (current_node) 
    { 
        ft_printf("list obj = %s\n", current_node->name);
        current_node = current_node->next;
    }
} //for test

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
    static int sub;
    static int last_have_file;
    static int first_dir;

   
    obj_list = NULL;
    total = get_list(&obj_list, arg_name, flags);//把所有的输入参数（名字）对象，建立一个链表
    // print_list(obj_list);
	sort_list(&obj_list, flags);
    // ft_printf("after_sort\n");
    // print_list(obj_list);
    if (sub && flags->R)//当R在子文件夹的状态下，显示各个子文件夹的名字
    {   
        if (first_dir == 0)
        {
            ft_printf("\n");
            ft_printf("%s:\n", arg_name);
            first_dir = 1;
        }
        else
        {
            if (last_have_file == 1)
                ft_printf("\n");
            ft_printf("%s:\n", arg_name);
        }
        if (total)
                last_have_file = 1;
            else if (!total && obj_list)
                last_have_file = 1;
            else
                last_have_file = 0;
    }
    if (obj_list && flags->l)
        ft_printf("total %d\n", total);
    check_list = obj_list;
	size = set_size(obj_list); //给t_size结构赋值，获得各个需要显示对象的长度len,用于-l时显示
    // ft_printf("w_cols = %d\n", size.w_cols);
    print_objs(check_list, size, flags);
    //下面如果R和l同时存在的话，会继续显示下一层的文件夹（实际没有了），从而产生不必要的报错。 
    if (total == 0 && errno == 13 && (flags->R || flags->l)) //如果之前读取这个文件夹时产生错误信息的情况下
    {
        while (*arg_name == '.'|| *arg_name == '/') //错误信息不需要显示./
            arg_name++;
        ft_printf("ls: %s: ", arg_name);
        perror(NULL); 
    }
    sub = 1;
	if (flags->R)
    {
        // if (size.f_name)
        //     ft_putchar('\n');  //如果文件夹里面没有文件名字的话，就不需要回车
        recur(check_list, flags);
    }
	free_list(&obj_list);
}


