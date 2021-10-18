/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/15 17:17:11 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <libft.h>
#include <pipex.h>

#include <debug.h>

#define OK 0

char	**create_path_split(char **envp)
{
	char	*curr_envp;
	int		i;

	i = 0;
	while (envp && envp[i])
	{
		curr_envp = ft_strnstr(envp[i], "PATH=", 5);
		if (curr_envp)
			break;
		i++;
	}
	return (ft_split(curr_envp + 5, ':'));
}

void	free_t_splits(t_splits *splits)
{
	if (splits->path_split)
		ft_free_split(splits->path_split);
	// if (splits->cmd1_split)
	// 	ft_free_split(splits->path_split);
	if (splits->cmd2_split)
		ft_free_split(splits->cmd2_split);
}

char	*get_correct_path(char *current_path, char *cmd)
{
	char	*full_cmd;
	char	*path;

	path = ft_strjoin(current_path, "/");
	full_cmd = ft_strjoin(path, cmd);
	// printf("checking current path->%s\n", full_cmd);
	if (access(full_cmd, X_OK) == OK)
	{
		free(path);
		return (full_cmd);
	}
	free(full_cmd);
	free(path);
	return (NULL);
}

char	*find_and_set_path(char *cmd, char **envp, t_splits *splits)
{
	char	**current_path;
	int		i;

	i = 0;
	splits->path_split = create_path_split(envp);
	if (!splits->path_split)
		return (NULL);
	current_path = splits->path_split;
	while (current_path[i])
	{
		if (get_correct_path(current_path[i], cmd))
		{
			// printf("working path: %s\n", current_path[i]);
			return (get_correct_path(current_path[i], cmd));
		}
		i++;
	}
	return (NULL);
}