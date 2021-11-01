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

#include <debug.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>

t_err	print_errno_string(t_err error, char *str)
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
	if (error == NO_CMD)
		printf("%s command not found\n", err_str);
	else
		perror(err_str);
	free(err_str);
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
		print_errno_string(return_value, NULL);
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
	free_heap(&pipex.heap);
	return (0);
}
