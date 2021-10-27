/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 21:15:19 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/27 03:44:11 by jcorneli         ###   ########.fr       */
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
	return (NO_CMD);
}

t_err	check_set_cmd(char *cmd_arg, char **path_split, char **full_cmd)
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

t_err	parse_files(char *in_arg, char *out_arg, char **infile, char **outfile)
{
	if (access(in_arg, R_OK) != OK)
		return (print_errno_string(NO_ACCESS, in_arg));
	*infile = ft_strdup(in_arg);
	if (!*infile)
		return (MALLOC_FAIL);
	if (access(out_arg, F_OK) == OK && access(out_arg, W_OK) != OK)
		return (print_errno_string(NO_ACCESS, out_arg));
	else
		*outfile = ft_strdup(out_arg);
	if (!*outfile)
		return (MALLOC_FAIL);
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
	if (parse_files(argv[1], argv[argc - 1], &pipex->heap.infile, \
		&pipex->heap.outfile) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	i = 2;
	while (i < argc - 1)
	{
		if (create_cmd_split(argv[i], \
			&splits->cmd_split[i - 2]) == MALLOC_FAIL)
			return (MALLOC_FAIL);
		if (check_set_cmd(splits->cmd_split[i - 2][0], splits->path_split, \
			&pipex->heap.command[i - 2]) == MALLOC_FAIL)
			return (MALLOC_FAIL);
		i++;
	}
	return (NO_ERROR);
}
