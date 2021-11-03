/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 21:15:19 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/03 23:14:05 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <libft.h>
#include <pipex.h>
#include <path.h>

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
		free(path);
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
		err = correct_path(path_split[i], cmd_info->cmd_split[0], \
				&cmd_info->full_cmd);
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
	if (ft_strncmp(f_info->infile, "here_doc", 9) == OK)
		f_info->here_doc = TRUE;
	else if (access(in_arg, R_OK) != OK)
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

t_err	parse_commands(int argc, char **argv, t_pipex *pipex)
{
	t_fork_info	f_info;
	int			i;
	int			rv;

	i = 0;
	f_info = pipex->fork_info;
	while (i < pipex->child_amount)
	{
		if (create_cmd_split(argv[i], &pipex->cmd_info[i].cmd_split) \
				== MALLOC_FAIL)
			return (MALLOC_FAIL);
		rv = check_set_cmd(pipex->fork_info.path_split, &pipex->cmd_info[i]);
		if (rv == MALLOC_FAIL)
			return (MALLOC_FAIL);
		else if (rv == NO_CMD && !((i == 2 && f_info.access_infile == FALSE) \
				|| (i == argc - 2 && f_info.access_outfile == FALSE)))
			create_errno_string(NO_CMD, pipex->cmd_info[i].cmd_split[0]);
		i++;
	}
	return (NO_ERROR);
}

t_err	parse_input(int argc, char **argv, char **envp, t_pipex *pipex)
{
	t_fork_info	*f_info;

	if (argc < 4)
		return (NO_ARGS);
	f_info = &pipex->fork_info;
	pipex->child_amount = argc - 3;
	pipex->pipe_amount = argc - 4;
	if (create_path_split(envp, &f_info->path_split) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	if (parse_files(argv[1], argv[argc - 1], &pipex->fork_info) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	return (parse_commands(argc, argv + 2, pipex));
}
