/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 21:15:19 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/01 13:18:35 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <libft.h>
#include <pipex.h>
#include <path.h>

#include <debug.h>

#define OK 0

t_err	correct_path(char *current_path, char *cmd, char **cmd_ptr)
{
	char	*full_cmd;
	char	*path;

	path = ft_strjoin(current_path, "/");
	if (!path)
		return (MALLOC_FAIL);
	full_cmd = ft_strjoin(path, cmd);
	if (!full_cmd)
	{
		free (path);
		return (MALLOC_FAIL);
	}
	if (access(full_cmd, X_OK) == OK)
	{
		free(path);
		*cmd_ptr = full_cmd;
		return (NO_ERROR);
	}
	free(full_cmd);
	free(path);
	*cmd_ptr = ft_strdup(cmd);
	if (!*cmd_ptr)
		return (MALLOC_FAIL);
	return (NO_CMD);
}

t_err	check_set_cmd(char **path_split, t_cmd_info *cmd_info)
{
	int		i;
	t_err	err;

	i = 0;
	while (path_split[i])
	{
		err = correct_path(path_split[i], cmd_info->cmd_split[0], &cmd_info->full_cmd);
		if (err == NO_ERROR)
			return (NO_ERROR);
		else if (err == MALLOC_FAIL)
			return (MALLOC_FAIL);
		i++;
	}
	cmd_info->cmd_not_found = TRUE;
	return (NO_CMD);
}

t_err	create_cmd_split(char *cmd_arg, char ***cmd_split)
{
	*cmd_split = ft_split(cmd_arg, ' ');
	if (*cmd_split)
		return (NO_ERROR);
	else
		return (MALLOC_FAIL);
}

t_err	parse_files(char *in_arg, char *out_arg, t_fork_info *f_info)
{
	f_info->infile = ft_strdup(in_arg);
	if (!f_info->infile)
		return (MALLOC_FAIL);
	if (access(in_arg, R_OK) != OK)
		create_errno_string(NO_ACCESS, in_arg);
	else
		f_info->access_infile = TRUE;
	if (access(out_arg, F_OK) == OK && access(out_arg, W_OK) != OK)
		return (create_errno_string(NO_ACCESS, out_arg));
	else
		f_info->outfile = ft_strdup(out_arg);
	if (!f_info->outfile)
		return (MALLOC_FAIL);
	f_info->access_outfile = TRUE;
	return (NO_ERROR);
}

t_err	parse_input(int argc, char **argv, char **envp, t_pipex *pipex)
{
	t_fork_info	*f_info;
	int			rv;
	int			i;

	f_info = &pipex->fork_info;
	if (create_path_split(envp, &f_info->path_split) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	if (parse_files(argv[1], argv[argc - 1], &pipex->fork_info) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	i = 0;
	while (i < argc - 3)
	{
		if (create_cmd_split(argv[i + 2], &pipex->cmd_info[i].cmd_split) == MALLOC_FAIL)
			return (MALLOC_FAIL);
		rv = check_set_cmd(pipex->fork_info.path_split, &pipex->cmd_info[i]);
		if (rv == MALLOC_FAIL)
			return (MALLOC_FAIL);
		else if (rv == NO_CMD && !((i == 2 && f_info->access_infile == FALSE) || (i == argc - 2 && f_info->access_outfile == FALSE)))
			create_errno_string(NO_CMD, pipex->cmd_info[i].cmd_split[0]);
		i++;
	}
	return (NO_ERROR);
}
