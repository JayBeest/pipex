/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/26 00:55:57 by jcorneli         ###   ########.fr       */
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

int	main(int argc, char **argv, char **envp)
{
	t_pipex		pipex;

	printf("MAIN_PID: %d\n", getpid());
	print_split(envp);
	ft_bzero(&pipex, sizeof(pipex));
	if (parse_input(argc, argv, envp, &pipex) != 0)
	{
		perror(NULL);
		return (1);
	}
	if (create_forks(&pipex) != 0)
	{
		perror(NULL);
		return (3);
	}
	if (pipex.fork_info.pid != 0)
		wait_and_free(&pipex);
	return (0);
}
