/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/01 11:54:48 by jcorneli         ###   ########.fr       */
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

t_err	store_err_to_heap(t_err error, char *err_str, char **err_str_ptr)
{
	char	*temp_ptr;
	char	*errno_str_ptr;

	temp_ptr = err_str;
	errno_str_ptr = strerror(errno);
	if (error == NO_CMD)
		err_str = ft_strjoin(err_str, ": command not found\n");
	else
	{
		err_str = ft_strjoin(err_str, ": ");
		ft_delstr(temp_ptr);
		if (!err_str)
			return (MALLOC_FAIL);
		temp_ptr = err_str;
		err_str = ft_strjoin(err_str, errno_str_ptr);
	}
	if (!err_str)
		{
			ft_delstr(temp_ptr);
			return (MALLOC_FAIL);
		}
	ft_delstr(*err_str_ptr);
	*err_str_ptr = err_str;
	// printf("TESTING CUSTOM ERRSTR = %s\n", *err_str_ptr);
	return (NO_ERROR);	
}

t_err	create_errno_string(t_err error, char *str, char **err_str_ptr)
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
	store_err_to_heap(error, err_str, err_str_ptr);
	// if (error == NO_CMD)
	// 	printf("%s command not found\n", err_str);
	// else
	// 	perror(err_str);
	// free(err_str);
	return (error);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	t_err	return_value;

//	printf("MAIN_PID: %d\n", getpid());
//	print_split(envp);
	ft_bzero(&pipex, sizeof(pipex));
	return_value = parse_input(argc, argv, envp, &pipex);
	if (return_value != 0)
		create_errno_string(return_value, NULL, &pipex.heap.errno_str);
	if (return_value == MALLOC_FAIL)
	{
		free_heap(&pipex.heap);
		return (1);
	}
	return_value = create_forks(&pipex);
	if (return_value > MALLOC_FAIL)
		return (3);
	if (pipex.fork_info.pid == 0)
		return (0);
	wait_for_children(&pipex);
	printf("%s", pipex.heap.errno_str);
	free_heap(&pipex.heap);
	return (0);
}
