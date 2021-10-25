/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 21:15:19 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/26 00:05:25 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <libft.h>
#include <pipex.h>
#include <path.h>

#include <debug.h>

#define OK 0

char	*correct_path(char *current_path, char *cmd)
{
	char	*full_cmd;
	char	*path;

	path = ft_strjoin(current_path, "/");
	full_cmd = ft_strjoin(path, cmd);
	printf("checking current path->%s\n", full_cmd);
	if (access(full_cmd, X_OK) == OK)
	{
		free(path);
		return (full_cmd);
	}
	free(full_cmd);
	free(path);
	return (NULL);
}

int	check_set_command(char *cmd_arg, char **path_split, char **full_cmd)
{
	int		i;

	i = 0;
	while (path_split[i])
	{
		*full_cmd = correct_path(path_split[i], cmd_arg);
		if (*full_cmd)
		{
			// printf("working path: %s\n", current_path[i]);
			return (0);
		}
		i++;
	}
	return (-1);
}

int	create_cmd_split(char *cmd_arg, char ***cmd_split)
{
	// printf("create_cmd_split->cmd_arg= %s\n", cmd_arg);
	*cmd_split = ft_split(cmd_arg, ' ');
	// print_split(*path_split);
	if (*cmd_split)
		return (0);
	else
		return (-1);
}

int	parse_files(char *in_arg, char *out_arg, char **infile, char **outfile)
{
	if (access(in_arg, R_OK) != OK)
		return (1);
	*infile = ft_strdup(in_arg);
	if (access(out_arg, F_OK | W_OK) == OK || access(out_arg, F_OK) != OK)
		*outfile = ft_strdup(out_arg);
	else
		return (2);
	return (0);
}

int	parse_input(int argc, char **argv, char **envp, t_pipex *pipex)
{
	int	i;

	pipex->process_amount = argc - 3;
	pipex->pipe_amount = argc - 4;
	if (create_path_split(envp, &pipex->heap.splits.path_split) == -1)
		return (1);
	if (parse_files(argv[1], argv[argc - 1], &pipex->heap.infile, &pipex->heap.outfile) != 0)
	 	return (2);
	printf("infile = %s\noutfile = %s\n", pipex->heap.infile, pipex->heap.outfile);
	i = 2;
	while (i < argc - 1)
	{
		if (create_cmd_split(argv[i], &pipex->heap.splits.cmd_split[i - 2]) == -1)
			return (3);
		if (check_set_command(pipex->heap.splits.cmd_split[i - 2][0], pipex->heap.splits.path_split, &pipex->heap.command[i - 2]) == -1)
			return (printf("CHECK_SET_CMD FAIL!!!!!\n"));;
		i++;
	}
	return (0);
}