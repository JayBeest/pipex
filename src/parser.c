/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 21:15:19 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/02 02:27:16 by jcorneli         ###   ########.fr       */
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

t_err	check_set_cmd(char *cmd_arg, char **path_split, char **full_cmd, t_bool *cmd_not_found)
{
	int		i;
	t_err	err;

	i = 0;
	while (path_split[i])
	{
		err = correct_path(path_split[i], cmd_arg, full_cmd);
		if (err == NO_ERROR)
			return (NO_ERROR);
		else if (err == MALLOC_FAIL)
			return (MALLOC_FAIL);
		i++;
	}
	*cmd_not_found = TRUE;
	return (print_errno_string(NO_CMD, cmd_arg));
}

t_err	create_cmd_split(char *cmd_arg, char ***cmd_split)
{
	*cmd_split = ft_split(cmd_arg, ' ');
	if (*cmd_split)
		return (NO_ERROR);
	else
		return (MALLOC_FAIL);
}

t_err	parse_files(char *in_arg, char *out_arg, t_heap *heap, \
		t_fork_info *f_info)
{
	heap->infile = ft_strdup(in_arg);
	if (!heap->infile)
		return (MALLOC_FAIL);
	if (access(in_arg, R_OK) != OK)
		print_errno_string(NO_ACCESS, in_arg);
	else
		f_info->access_infile = TRUE;
	if (access(out_arg, F_OK) == OK && access(out_arg, W_OK) != OK)
		return (print_errno_string(NO_ACCESS, out_arg));
	else
		heap->outfile = ft_strdup(out_arg);
	if (!heap->outfile)
		return (MALLOC_FAIL);
	f_info->access_outfile = TRUE;
	return (NO_ERROR);
}

t_err	parse_input(int argc, char **argv, char **envp, t_pipex *pipex)
{
	t_splits	*splits;
	int	i;

	pipex->child_amount = argc - 3;
	pipex->pipe_amount = argc - 4;
	splits = &pipex->heap.splits;
	if (create_path_split(envp, &splits->path_split) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	if (parse_files(argv[1], argv[argc - 1], &pipex->heap, \
		&pipex->fork_info) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	i = 2;
	while (i < argc - 1)
	{
		if (create_cmd_split(argv[i], \
			&splits->cmd_split[i - 2]) == MALLOC_FAIL)
			return (MALLOC_FAIL);
		if (check_set_cmd(splits->cmd_split[i - 2][0], splits->path_split, \
			&pipex->heap.command[i - 2], &pipex->fork_info.cmd_not_found[i - 2]) == MALLOC_FAIL)
			return (MALLOC_FAIL);
		i++;
	}
	return (NO_ERROR);
}
