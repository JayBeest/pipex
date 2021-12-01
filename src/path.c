/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 22:14:40 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/25 00:19:59 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <pipex.h>

t_err	create_path_split(char **envp, char ***path_split)
{
	char	*curr_envp;
	int		i;

	i = 0;
	if (!envp)
		return (ENVP_FAIL);
	while (envp[i])
	{
		curr_envp = ft_strnstr(envp[i], "PATH=", 5);
		if (curr_envp)
			break ;
		i++;
	}
	*path_split = ft_split(curr_envp + 5, ':');
	if (!*path_split)
		return (MALLOC_FAIL);
	return (NO_ERROR);
}

t_err	correct_path(char *current_path, char *cmd, char **cmd_ptr)
{
	char	*full_cmd;
	char	*path;

	path = ft_strjoin(current_path, "/");
	if (!path)
		return (MALLOC_FAIL);
	full_cmd = ft_strjoin(path, cmd);
	free(path);
	if (!full_cmd)
		return (MALLOC_FAIL);
	if (access(full_cmd, X_OK) == OK)
	{
		*cmd_ptr = full_cmd;
		return (NO_ERROR);
	}
	free(full_cmd);
	return (NO_CMD);
}

t_err	absolute_relative_path(t_cmd_info *cmd_info)
{
	if (access(cmd_info->cmd_split[0], X_OK) == OK)
	{
		cmd_info->full_cmd = ft_strdup(cmd_info->cmd_split[0]);
		if (!cmd_info->full_cmd)
			return (MALLOC_FAIL);
		return (NO_ERROR);
	}
	if (access(cmd_info->cmd_split[0], F_OK) == OK)
	{
		cmd_info->permission_denied = 1;
		return (NO_ACCESS);
	}
	cmd_info->cmd_not_found = 1;
	return (NO_CMD);
}

t_err	check_set_cmd(char **path_split, t_cmd_info *cmd_info)
{
	int		i;
	t_err	err;
	char	*cmd;

	cmd = cmd_info->cmd_split[0];
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/') || \
		(cmd[0] == '~' && cmd[1] == '/') || \
		(cmd[0] == '.' && cmd[1] == '.' && cmd[2] == '/'))
		return (absolute_relative_path(cmd_info));
	i = 0;
	while (path_split[i])
	{
		err = correct_path(path_split[i], cmd, &cmd_info->full_cmd);
		if (err == NO_ERROR)
			return (NO_ERROR);
		else if (err == MALLOC_FAIL)
			return (MALLOC_FAIL);
		i++;
	}
	cmd_info->cmd_not_found = TRUE;
	return (NO_CMD);
}
