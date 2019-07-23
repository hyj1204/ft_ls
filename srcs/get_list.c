/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 22:34:48 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/22 20:48:56 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int			get_list(t_arg **obj_list, char *arg_name, t_flag *flags) 
//输入名字和flag把所有的要用到的对象做成一个链表list
{
	DIR				*dir; //opendir()返回的结构用于readdir()和closedir()
    struct dirent	*ent; //通过readdir(DIR) 获得指向dirent结构体的指针。用来获得里面的d_name，用在之后的stat函数里。
	int				total;

	total = 0;
	errno = 0;
	if (!(dir = opendir(arg_name))) //如果打开参数名字失败（该文件夹名字不存在/这是一个文件/有权限（13代表没有权限）的时候）
	{
		if (errno == 2)//arg_name名字不存在（错误值2）的时候
		{
			ft_printf("ls: %s: ", arg_name);
			perror(NULL); //当没有这个文件名的时候，显示错误信息,结尾会有回车
		}
		else if (flags->R || flags->l)
		{
			errno;
		}
		else if (!(flags->R) && errno == 13)
		{
			while (*arg_name == '.'|| *arg_name == '/') //错误信息不需要显示./
            	arg_name++;
        	ft_printf("ls: %s: ", arg_name);
        	perror(NULL); 
			// ft_printf("ls: %s: %s", arg_name, strerror(errno));
			//当没有权限接入的时候，显示报错信息，结尾没有回车
		}
		else
		{
			save_ftolist(obj_list, arg_name); //把所有只要名字存在的文件（非文件夹）放入list
		}
		return (0);
	}
    //在opendir成功的情况下（即arg_name是文件夹名字），读取文件夹里面每一个文件信息（包括隐藏文件）
	//如果文件夹没有权限接入的时候，也会走到这里，保存文件夹名字，具体read的时候再显示没有权限的错误信息.
	while ((ent = readdir(dir)) != NULL) //readdir(dir) 会一直读取输入文件夹dir里,所有对象（包括隐藏文件/普通文件/文件夹/没有权限读取的对象）
	{
		// printf("d_name = %s\n", ent->d_name);
		if (ent->d_name[0] != '.' || flags->a) //如果当前对象不是隐藏文件，或者是-a需要显示隐藏文件的时候
			total += save_dtolist(obj_list, arg_name, ent); //就把这个对象放入 输入的list里面。total是所有文件占用磁盘块的总和
	}
	closedir(dir);
	// ft_printf("total = %d\n", total);
	return (total);
}


int	save_ftolist(t_arg **obj_list, char *file_name)
{
	t_arg	*tmp;

	if (!(tmp = ft_memalloc(sizeof(t_arg))))
	{
        ft_printf("save_file_to_list memallc fail\n");
        return (1);
    }
	// ft_printf("file_name = %s\n", file_name);
	tmp->name = ft_strdup(file_name);
	tmp->path = ft_strdup(file_name); //保存路径只是为了显示错误信息
	if (lstat(tmp->path, &(tmp->info)) == -1) 
	{
		// ft_printf("save_ftolist fial"); 
		ft_printf("ls: "); 
		perror(tmp->path);
		free(tmp->name);
		free(tmp->path);
		free(tmp);
		return (0);
	}
	tmp->is_dir = 0;
	// ft_printf("file_tmp = %s\n", tmp->name);
	add_list(obj_list, tmp);
	return (tmp->info.st_blocks);
}


int	save_dtolist(t_arg **list, char *dir_name, struct dirent *ent)
//list保存的所有需要显示出来的（文件夹内）对象基础信息
{
	t_arg	*tmp; //tmp是个指向t_arg的指针，所以需要malloc先获得存放内容的内存。

	if (!(tmp = ft_memalloc(sizeof(t_arg)))) //分配全0空白内存空间
    {
        ft_printf("save_list memallc fail\n");
        return (1);
    }
	tmp->name = ft_strdup(ent->d_name);
	tmp->path = fix_path(dir_name, ent->d_name);
	// printf("dir path = %s\n", tmp->path);
    //输入文件夹名字 和 文件夹下对象名字 获得路径.用于-R显示文件路径信息
	//例如：123/a.out
	if (lstat(tmp->path, &(tmp->info)) == -1) //读取参数文件夹下的对象stat结构，存放在tmp->info里
	{
		ft_printf("ls: ");
		// perror("save_dtolist lstat fail");
		free(tmp->name);
		free(tmp->path);
		free(tmp);
		return (0);
	}
	if (S_ISDIR(tmp->info.st_mode) == 1)
		tmp->is_dir = 1;
	add_list(list, tmp);
	return (tmp->info.st_blocks);  //返回该文件所占的磁盘块number of 512B blocks allocated
}


//输入文件夹名 ./123 效果和 123 相同
//ls ~ 是列出主路径的所有文件
char	*fix_path(char *dir_name, char *d_name)
{
	char	*tmp;
	char	*tmp2;

	if (!d_name)
	{
		if (dir_name[0] == '~' || dir_name[0] == '/' || dir_name[0] == '.')
			return (ft_strdup(dir_name));
		return (ft_strjoin("./", dir_name));
	}

	tmp = ft_strjoin(dir_name, "/");
	tmp2 = ft_strjoin(tmp, d_name);

	// ft_printf("path = %s\n", tmp2);
	free(tmp);
	return (tmp2);
}

void	add_list(t_arg **list, t_arg *tmp)
{
	tmp->next = *list;
	*list = tmp;
}