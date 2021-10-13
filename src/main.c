/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/13 09:35:42 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <main.h>
#include <libft.h>

#include <debug.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>

#define OK 0

int	parent_fork(char **argv, char **envp, int pipe_fd[2])
{
	int	fd;

	close(pipe_fd[1]);
	wait(NULL);
	fd = open(OUTFILE, O_CREAT | O_WRONLY);
	// if (access(OUTFILE, F_OK | W_OK) == OK)
	// 	fd = open(OUTFILE, O_CREAT | O_WRONLY);
	// else
	// 	return (printf("noooooooo no access_OUTFILE!\n"));
	if (fd == -1)
		return (printf("ooooohnoo, open_OUTFILE failed\n"));
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (printf("nooooo dup2_OUTFILE failed"));
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		return (printf("fuck your dup2_stdout :/\n"));
	close (pipe_fd[0]);
	printf("PARENT execv_return = %d", execv(CMD2, ft_split(CMD2, ' ')));
	printf("PARENT done, closing pipe_fd[1]\n");
	return (0);
}

int	child_fork(char **argv, char **envp, int pipe_fd[2])
{
	int	fd;

	close(pipe_fd[0]);
	if (access(INFILE, F_OK | R_OK) == OK)
		fd = open(INFILE, O_RDONLY);
	else
		return (printf("noooooooo no access_INFILE!\n"));
	if (fd == -1)
		return (printf("ooooohnoo, open_INFILE failed\n"));
	if (dup2(fd, STDIN_FILENO) == -1)
		return (printf("nooooo dup2_INFILE failed"));
	close(fd);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		return (printf("hell no, dup2_pipe_fd[1] fail :(\n"));
	close (pipe_fd[1]);
	printf("CHILD execv_return = %d",execv(CMD1, ft_split(CMD1, ' ')));

	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	int			pid;
	int			pipe_fd[2];

	if (argc != 5)
		return (printf("argc not 5!!!\n"));
	// print_envp(envp);
	// check_access(INFILE);
	if (pipe(pipe_fd) == -1)
		return (printf("creating pipe failed! :(\n"));
	pid = fork();
	if (pid == -1)
		return (printf("forking failed :(((\n"));
	if (pid == 0)
		child_fork(argv, envp, pipe_fd);
	else
		parent_fork(argv, envp, pipe_fd);
	// if (pid == 0)
	// 	close(pipe_fd[1]);
	// else
	// 	close(pipe_fd[0]);

	// system("leaks pipex");
	return (0);
}