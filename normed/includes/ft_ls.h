/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 17:34:43 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/31 01:12:48 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# define MAX(x, y) x > y ? x : y

# include "libft.h"
# include "ft_printf.h"
# include <limits.h>
# include <dirent.h>
# include <sys/stat.h>
# include <errno.h>
# include <sys/ioctl.h>
# include <stdio.h>
# include <sys/types.h>
# include <pwd.h>
# include <uuid/uuid.h>
# include <grp.h>
# include <time.h>
# include <sys/xattr.h>
# include <sys/acl.h>
# include <stdint.h>
# include <unistd.h>

typedef struct	s_flag
{
	uint8_t		a:1;
	uint8_t		l:1;
	uint8_t		u_r:1;
	uint8_t		r:1;
	uint8_t		t:1;
	uint8_t		u:1;
	uint8_t		one:1;
}				t_flag;

typedef struct	s_arg
{
	char			*name;
	char			*path;
	struct stat		info;
	uint8_t			is_dir;
	int				total;
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
	int			w_cols;
}				t_size;

int				g_amount;
char			g_nflag_char;
int				g_index;
int				ft_isdir(const char *path);
t_flag			init_flags(void);
char			get_opt(int ac, char **av, char *flags_str);
void			err_print(void);
t_flag			set_flags(char c, t_flag flags);
t_flag			getflags(int ac, char **av);
int				get_dir_list(t_arg **obj_list, char *arg_name, t_flag *flags);
int				save_ftolist(t_arg **obj_list, char *file_name);
int				save_dtolist(t_arg **list, char *dir_name, struct dirent *ent);
char			*fix_path(char *dir_name, char *d_name);
void			add_list(t_arg **list, t_arg *tmp);
int				ft_ls(char *arg_name, t_flag *flags);
void			free_list(t_arg **list);
void			print_list(t_arg *a, t_size size, t_flag *flags);
void			print_permissions(t_arg *list);
int				print_objs(t_arg *iter, t_size size, t_flag *flags);
int				get_line_capacity(int mlen, int cols);
t_size			set_size(t_arg *list);
int				ft_getwincols(void);
int				get_digits(int64_t n);
void			sort_av(int ac, char ***av, int start, t_flag *flags);
void			sort_list(t_arg **obj_list, t_flag *flags);
void			reverse_list(t_arg **obj_list);
void			merge_sort(t_arg **list, t_flag *flags);
void			split(t_arg *head, t_arg **a, t_arg **b);
t_arg			*lexical_sorted(t_arg *a, t_arg *b, t_flag *flags);
t_arg			*lexical_sorted2(t_arg *a, t_arg *b);
int				timecmp(t_arg *a, t_arg *b, t_flag *flags);

#endif
