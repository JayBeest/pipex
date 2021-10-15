/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/15 17:17:11 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <libft.h>
#include <pipex.h>
#include <utils.h>

#include <debug.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>

int	cmd2_parent_fork(char **argv, int pipe_fd[2], t_splits *splits, t_cmds cmd)
{
	int		fd;

	close(pipe_fd[1]);
//	if (access(OUTFILE, F_OK | W_OK) == -1)
//	 	return (printf("noooooooo no access_OUTFILE!\n"));
	fd = open(OUTFILE, O_RDWR | O_CREAT);
	if (fd == -1)
		return (printf("ooooohnoo, open_OUTFILE failed\n"));
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (printf("nooooo dup2_OUTFILE failed"));
	close(fd);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		return (printf("fuck your dup2_stdout :/\n"));
	close (pipe_fd[0]);
	splits->cmd2_split = ft_split(CMD2, ' ');
	wait(NULL);
 	execv(cmd.cmd2, splits->cmd2_split);
	return (0);
}

int	cmd1_child_fork(char **argv, int pipe_fd[2], t_splits *splits, t_cmds cmd)
{
	int	fd;

	close(pipe_fd[0]);
	if (access(INFILE, F_OK | R_OK) == -1)
		return (printf("noooooooo no access_INFILE!\n"));
	else
		fd = open(INFILE, O_RDONLY);
	if (fd == -1)
		return (printf("ooooohnoo, open_INFILE failed\n"));
	if (dup2(fd, STDIN_FILENO) == -1)
		return (printf("nooooo dup2_INFILE failed"));
	close(fd);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		return (printf("hell no, dup2_pipe_fd[1] fail :(\n"));
	close (pipe_fd[1]);
	splits->cmd1_split = ft_split(CMD1, ' ');
	execv(cmd.cmd1, ft_split(CMD1, ' '));
	return (0);
}



int	main(int argc, char **argv, char **envp)
{
	int			pid;
	int			pipe_fd[2];
	t_splits	splits;
	t_cmds		commands;

	if (argc != 5)
		return (printf("argc not 5!!!\n"));
//	print_split(envp);
//	check_access(INFILE);
	ft_bzero(&splits, sizeof(splits));

	commands.cmd1 = find_and_set_path(&CMD1, envp, &splits);
	commands.cmd2 = find_and_set_path(&CMD2, envp, &splits);
	if (pipe(pipe_fd) == -1)
		return (printf("creating pipe failed! :(\n"));
	pid = fork();
	if (pid == -1)
		return (printf("forking failed :(((\n"));
	if (pid == 0)
		cmd1_child_fork(argv, pipe_fd, &splits, commands);
	else
	{
		cmd2_parent_fork(argv, pipe_fd, &splits, commands);
		free_t_splits(splits);
	}
	// if (pid == 0)
	// 	close(pipe_fd[1]);
	// else
	// 	close(pipe_fd[0]);

//	system("leaks pipex");
	return (0);
}