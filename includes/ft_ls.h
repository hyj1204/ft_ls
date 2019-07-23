/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 17:34:43 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/22 21:27:56 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

#include "libft.h"
#include "ft_printf.h"

#include <limits.h> //for PATH_MAX
#include <dirent.h> //for opendir()
#include <sys/stat.h> //for stat() and related struct
#include <errno.h> //for errno
#include <sys/ioctl.h> //for TIOCGWINSZ
#include <stdio.h> //for perror
#include <sys/types.h> //for getpwuid
#include <pwd.h>
#include <uuid/uuid.h>
#include <grp.h> //for getgrgid
#include <time.h> //for ctime()
#include <sys/xattr.h> // for listxattr
#include <sys/acl.h>
#include <stdint.h> //for uint8
#include <unistd.h>
#include <stdlib.h>


typedef struct	s_flag
{
	uint8_t		a:1;
	uint8_t		l:1;
	uint8_t		R:1;
	uint8_t		r:1;
	uint8_t		t:1;
	uint8_t		one:1;
}				t_flag;

typedef struct	s_arg    //针对每个输入对象，保存这个对象信息（名字，路径，是否是文件夹，总内存是多少
{
	char			*name; //对象的名字
	char			*path;
	struct stat		info;  //输入对象的stat结构信息
	uint8_t			is_dir;
	int				total; //这个文件夹的总文件数
	struct s_arg	*next;
}				t_arg;

typedef struct	s_size
{
	int			nlink;
	int			pw_name;
	int			gr_name;
	int			st_size;
	int			major;
	int			minor;
	int			device;
	int			f_name;
	int			w_cols;//有多少列
}				t_size;

# define MAX(x, y) x > y ? x : y

int				f_amount;
//for get_flags
char			not_flag_char; //不是flag字符而是个普通字符,则用全局变量保存这个字符，用于之后显示报错信息函数用
int				arg_index;
t_flag			init_flags(void);  //初始化flags结构
char 			get_opt(int ac, char **av, char *flags_str); //有flag的情况下就会返回那个flag字符，没有的话就返回-1
void			err_print(void);
t_flag			set_flags(char c, t_flag flags); //把获得flag字符赋值到flags结构里
t_flag			getflags(int ac, char **av);

//get_list
int				get_list(t_arg **obj_list, char *arg_name, t_flag *flags);
int				save_ftolist(t_arg **obj_list, char *file_name);
int				save_dtolist(t_arg **list, char *dir_name, struct dirent *ent);
char			*fix_path(char *dir_name, char *d_name);
void			add_list(t_arg **list, t_arg *tmp);

//ft_ls
// void print_list(t_arg *head); //only for test
void 			ft_ls(char *arg_name, t_flag *flags);
void			free_list(t_arg **list);

//print_arg
void		print_arg(t_arg *a, t_size size, t_flag *flags);
void		print_permissions(t_arg *list);

//print_objs
void	print_objs(t_arg *iter, t_size size, t_flag *flags);
int		get_line_capacity(int mlen, int cols);

//set size
t_size	set_size(t_arg *list);
int		ft_getwincols(void);
int		get_digits(int64_t n);

//sort_av
void		sort_av(int ac, char ***av, int start, t_flag *flags);

//sort_list
void	sort_list(t_arg **obj_list, t_flag *flags);
void	reverse_list(t_arg **obj_list);
void	merge_sort(t_arg **list, t_flag *flags);
void	split(t_arg *head, t_arg **a, t_arg **b);
t_arg	*lexical_sorted(t_arg *a, t_arg *b, t_flag *flags);
int		timecmp(t_arg *a, t_arg *b, t_flag *flags);

#endif