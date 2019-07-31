/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yijhuang <yijhuang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 22:34:48 by yijhuang          #+#    #+#             */
/*   Updated: 2019/07/31 00:07:37 by yijhuang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int		get_dir_list(t_arg **obj_list, char *arg_name, t_flag *flags)
{
	DIR				*dir;
	struct dirent	*ent;
	int				total;

	total = 0;
	dir = opendir(arg_name);
	while ((ent = readdir(dir)) != NULL)
	{
		if (ent->d_name[0] != '.' || flags->a)
			total += save_dtolist(obj_list, arg_name, ent);
	}
	closedir(dir);
	return (total);
}

int		save_ftolist(t_arg **obj_list, char *file_name)
{
	t_arg	*tmp;

	if (!(tmp = ft_memalloc(sizeof(t_arg))))
		return (1);
	tmp->name = ft_strdup(file_name);
	tmp->path = ft_strdup(file_name);
	if (lstat(tmp->path, &(tmp->info)) == -1)
	{
		ft_printf("ls: ");
		perror(tmp->path);
		free(tmp->name);
		free(tmp->path);
		free(tmp);
		return (0);
	}
	tmp->is_dir = 0;
	add_list(obj_list, tmp);
	return (tmp->info.st_blocks);
}

int		save_dtolist(t_arg **list, char *dir_name, struct dirent *ent)
{
	t_arg	*tmp;

	if (!(tmp = ft_memalloc(sizeof(t_arg))))
		return (1);
	tmp->name = ft_strdup(ent->d_name);
	tmp->path = fix_path(dir_name, ent->d_name);
	if (lstat(tmp->path, &(tmp->info)) == -1)
	{
		ft_printf("ls: ");
		free(tmp->name);
		free(tmp->path);
		free(tmp);
		return (0);
	}
	if (S_ISDIR(tmp->info.st_mode) == 1)
		tmp->is_dir = 1;
	add_list(list, tmp);
	return (tmp->info.st_blocks);
}

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
	free(tmp);
	return (tmp2);
}

void	add_list(t_arg **list, t_arg *tmp)
{
	tmp->next = *list;
	*list = tmp;
}
