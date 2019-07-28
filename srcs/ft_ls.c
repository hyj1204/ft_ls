/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/07 13:17:12 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/28 01:42:38 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int	ft_isdir(const char *path) //检查这个路径的对象是不是一个文件夹
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}


void print_list_test(t_arg *head) {
    t_arg *current_node;
    current_node = head;
   	while (current_node) 
    { 
        ft_printf("list obj = %s\n", current_node->name);
        current_node = current_node->next;
    }
} //for test


//检查list里有没有文件夹，有的话，就继续显示这个文件夹里面的内容
static void		recur(t_arg *obj_list, t_flag *flags)
{
	while (obj_list)
	{
		if (obj_list->is_dir && ft_strcmp(obj_list->name, ".") && ft_strcmp(obj_list->name, ".."))
        //如果对象是一个文件夹的时候，同时不是隐藏文件的时候（跟隐藏文件比较，值不为0）
		{
			ft_putchar('\n'); //如果发现list有文件夹的话，就回车，然后后面ft_ls中显示子文件夹名。
			ft_ls(obj_list->path, flags); //再把文件路径放入ls函数中
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

static int			check_is_file(t_arg **obj_list, char *arg_name, t_flag *flags, int sub)
{
    DIR				*dir; //opendir()返回的结构用于readdir()和closedir()
    int             i;

    errno = 0;
    i = 0;
    if (!(dir = opendir(arg_name))) //如果打开参数名字失败（该文件夹名字不存在/这是一个文件/有权限（13代表没有权限）的时候）
	{
		if (errno == 2)//arg_name名字不存在（错误值2）的时候
		{
			ft_printf("ls: %s: ", arg_name);
			perror(NULL); //当没有这个文件名的时候，显示错误信息,结尾会有回车
		}
		else if ((flags->R || flags->l) && errno == 13 && ft_isdir(arg_name))
		{
            if (sub && flags->R)
                {
                    while (arg_name[i] && (arg_name[i] == '.' || arg_name[i] == '/'))
                        i++;
                    ft_printf("\n%s\nls: %s: %s", arg_name, &arg_name[i], strerror(errno));
                } //当R的时候，直接显示文件夹名和无权限报错信息。
            else
                ft_printf("ls: %s: %s", &arg_name[i], strerror(errno));
		}
		else if (!(flags->R) && errno == 13)
		{
			while (arg_name[i] && (arg_name[i] == '.' || arg_name[i] == '/'))
                        i++;
        	ft_printf("ls: %s: ", arg_name);
        	perror(NULL); 
			// ft_printf("ls: %s: %s", arg_name, strerror(errno));
			//当没有权限接入的时候，显示报错信息，结尾没有回车
		}
		else
		{
			save_ftolist(obj_list, arg_name); //把所有只要名字存在的文件（非文件夹）放入list
		}
		return  1;
	}
    return 0;
}


//输入文件夹名和flags，根据flag要求显示这个文件夹里的对象
int ft_ls(char *arg_name, t_flag *flags)
{
    t_arg	*check_list;
    t_arg   *obj_list;
    int     total;
    t_size  size;
    static int sub;

    total = 0;
    obj_list = NULL;
    if (check_is_file(&obj_list, arg_name, flags, sub))
        return 0;
    //如果输入的arg_name不是文件夹的话，就显示相应报错(名字不存在）或者把文件存入list（如果是一般文件的话）.
    total = get_dir_list(&obj_list, arg_name, flags);
    //如果是文件夹的话，就获得这个文件夹内的所有对象
    //并返回这个文件夹下所有对象的block总和
	sort_list(&obj_list, flags);
    if (sub && flags->R)//当R在子文件夹的状态下，显示各个子文件夹的名字
    {
        if (flags->l && !obj_list) //当R和l同时存在，并且是空文件夹的时候
            ft_printf("\n%s:", arg_name);
        else
            ft_printf("\n%s:\n", arg_name);
    }
    if (obj_list && flags->l) //如果是空文件夹的情况就不需要显示total
        ft_printf("total %d\n", total);
    check_list = obj_list;
	size = set_size(obj_list); //给t_size结构赋值，获得各个需要显示对象的长度len,用于-l时显示
    // ft_printf("w_cols = %d\n", size.w_cols);
    print_objs(check_list, size, flags);
    //下面如果R和l同时存在的话，会继续显示下一层的文件夹（实际没有了），从而产生不必要的报错。 
    // if (total == 0 && errno == 13 && (flags->R || flags->l)) //如果之前读取这个文件夹时产生错误信息的情况下
    // {
    //     while (*arg_name == '.'|| *arg_name == '/') //错误信息不需要显示./
    //         arg_name++;
    //     ft_printf("ls: %s: ", arg_name);
    //     perror(NULL); 
    // }
    sub = 1;
	if (flags->R)
        recur(check_list, flags);
	free_list(&obj_list);
    return 0;
}


