/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/03 00:56:16 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <libft.h>
#include <pipex.h>
#include <parser.h>
#include <path.h>
#include <forks.h>
#include <utils.h>
#include <string.h>

#include <debug.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>

t_err	print_errno_string(t_err error, char *err_str)
{
	char	*temp_ptr;

	temp_ptr = err_str;
	if (error == NO_CMD)
	{
		err_str = ft_strjoin(err_str, ": command not found");
		if (!err_str)
		{
			ft_delstr(temp_ptr);
			return (MALLOC_FAIL);
		}
		ft_putendl_fd(err_str, 2);
	}
	else
		perror(err_str);
	return (error);
}

t_err	create_errno_string(t_err error, char *str)
{
	char	*temp_str;
	char	*err_str;

	if (error > NO_ERROR)
		temp_str = ft_strdup("pipex: ");
	else
		temp_str = ft_strdup(str);
	if (!temp_str)
		return (MALLOC_FAIL);
	err_str = ft_strjoin(temp_str, str);
	if (!err_str)
	{
		free(temp_str);
		return (MALLOC_FAIL);
	}
	free(temp_str);
	print_errno_string(error, err_str);
	free(err_str);
	return (error);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	t_err	return_value;
	int		exit_code;

	pipex.fork_info.delimiter = ft_strdup("DELIMITER");
	ft_bzero(&pipex, sizeof(pipex));
	pipex.child_amount = argc - 3;
	pipex.pipe_amount = argc - 4;
	return_value = parse_input(argc, argv, envp, &pipex);
	if (return_value != 0)
		create_errno_string(return_value, NULL);
	if (return_value == MALLOC_FAIL)
	{
		free_heap(&pipex);
		return (-1);
	}
	return_value = create_forks(&pipex);
	if (return_value > MALLOC_FAIL)
		return (-2);
	if (pipex.fork_info.pid == 0)
		return (0);
	exit_code = wait_for_children(&pipex);
	free_heap(&pipex);
	return (exit_code);
}
