/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/20 03:13:58 by jcorneli         ###   ########.fr       */
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

int	cmd2_child_fork(char **argv, int pipe_fd[2], t_splits *splits, t_cmds cmd)
{
	int		fd;

	printf("cmd2_parent_fork pid: %d, ppid: %d\n", getpid(), getppid());

	fd = open(OUTFILE, O_RDWR | O_CREAT, 0662);
	if (fd == -1)
		return (printf("ooooohnoo, open_OUTFILE failed\n"));
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (printf("nooooo dup2_OUTFILE failed"));
	close(fd);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		return (printf("fuck your dup2_stdout :/\n"));
	close(pipe_fd[1]);
	close(pipe_fd[0]);
 	execv(cmd.cmd2, splits->cmd2_split);
	return (0);
}

int	cmd1_child_fork(char **argv, int pipe_fd[2], t_splits *splits, t_cmds cmd)
{
	int	fd;

	printf("cmd1_child_fork pid: %d, ppid: %d\n", getpid(), getppid());

	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		return (printf("hell no, dup2_pipe_fd[1] fail :(\n"));
	if (access(INFILE, F_OK | R_OK) == -1)
		return (printf("noooooooo no access_INFILE!\n"));
	else
		fd = open(INFILE, O_RDONLY);
	if (fd == -1)
		return (printf("ooooohnoo, open_INFILE failed\n"));
	if (dup2(fd, STDIN_FILENO) == -1)
		return (printf("nooooo dup2_INFILE failed"));
	close(fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	execv(cmd.cmd1, ft_split(CMD1, ' '));
	return (0);
}

int	create_splits(char **argv, char **envp, t_splits *splits, t_cmds *commands)
{
	splits->cmd1_split = ft_split(CMD1, ' ');
	if (splits->cmd1_split == NULL)
		return (printf("cmd1_split fail\n"));
	splits->cmd2_split = ft_split(CMD2, ' ');
	if (splits->cmd1_split == NULL)
		return (printf("cmd2_split fail\n"));
	commands->cmd1 = find_and_set_path(splits->cmd1_split[0], envp, splits);
	if (commands->cmd1 == NULL)
		return (printf("cmd1 fail!\n"));
	commands->cmd2 = find_and_set_path(splits->cmd2_split[0], envp, splits);
	if (commands->cmd2 == NULL)
		return (printf("cmd2 fail!\n"));
	return (0);
}

int	main2(int argc, char **argv, char **envp)
{
	int			pid[2];
	int			pipe_fd[2];
	t_splits	splits;
	t_cmds		commands;

	if (argc != 5)
		return (printf("argc not 5!!!\n"));
//	print_split(envp);
//	check_access(INFILE);
	ft_bzero(&splits, sizeof(splits));
	if (create_splits(argv, envp, &splits, &commands) != 0)
		return (printf("mis_split!\n"));
	if (pipe(pipe_fd) == -1)
		return (printf("creating pipe failed! :(\n"));
	pid[0] = fork();
	if (pid[0] == -1)
		return (printf("fork[0] failed :(((\n"));
	if (pid[0] == 0)
		cmd1_child_fork(argv, pipe_fd, &splits, commands);
	else
	{
		printf("child_ID[0]? %d\n", pid[0]);
		pid[1] = fork();
		if (pid[1] == -1)
			return (printf("fork[1] failed :(((\n"));
		if (pid[1] == 0)
			cmd2_child_fork(argv, pipe_fd, &splits, commands);
		if (pid[0] != 0 && pid[1] != 0)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			wait(NULL);
			wait(NULL);
			free_t_splits(&splits);
		}
	}
	return (0);
}

int	create_pipes(int (*pipe_fd)[500][2], int pipe_amount)
{
	int	i;

	i = 0;
	while (i < pipe_amount)
	{
		if (pipe((*pipe_fd)[i]) == -1)
			return (-1);
		i++;
	}
	i = 0;
	while (i < pipe_amount)
	{
		printf("fd[%d][0] = %2d  fd[%d][1] = %2d\n", i, (*pipe_fd)[i][0], i, (*pipe_fd)[i][1]);
		i++;
	}
	return (0);
}

int	fork_start(int pipe_fd1[2], int *pid, t_heap *heap)
{
	int	fd;

	*pid = fork();
	if (*pid == -1)
		return (-1);
	if (*pid == 0)
		{
			printf("start_child %d here->fd1[0] = %2d fd1[1] = %2d\n", getpid(), pipe_fd1[0], pipe_fd1[1]);
			close(pipe_fd1[0]);
			if (dup2(pipe_fd1[1], STDOUT_FILENO) == -1)
				return (1);
			close(pipe_fd1[1]);
			fd = open("infile", O_RDONLY);
			if (fd == -1)
				return (2);
			if (dup2(fd, STDIN_FILENO) == -1)
				return (3);
			close(fd);
			execv(heap->commands.cmd1, heap->splits.cmd1_split);
		}
	return (0);
}

int	fork_end(int pipe_fd2[2], int *pid, t_heap *heap, int *last_pid)
{
	*pid = fork();
	if (*pid == -1)
		return (-1);
	if (*pid == 0)
		{
			printf("end_child   %d here->fd2[0] = %2d fd2[1] = %2d\n", getpid(), pipe_fd2[0], pipe_fd2[1]);
		}
	else
		*last_pid = *pid;
	return (0);
}

int	fork_mid(int pipe_fd1[2], int pipe_fd2[2], int *pid, t_heap *heap)
{
	*pid = fork();
	if (*pid == -1)
		return (-1);
	if (*pid == 0)
		{
			printf("mid_child   %d here->fd1[0] = %2d fd1[1] = %2d  fd2[0] = %2d fd2[1] = %2d\n", getpid(), pipe_fd1[0], pipe_fd1[1], pipe_fd2[0], pipe_fd2[1]);
		}
	return (0);
}

int	create_forks(t_pipex *pipex)
{
	int			i;
	t_fork_info	*f_info;

	f_info = &pipex->fork_info;
	i = 0;
	while (i < pipex->process_amount)
	{
		if (i == 0)
			f_info->rv = fork_start(f_info->fd[i], &f_info->pid[i], &pipex->heap);
		else if (i == pipex->process_amount - 1)
			f_info->rv = fork_end(f_info->fd[i - 1], &f_info->pid[i], &pipex->heap, &pipex->fork_info.last_child);
		else
			f_info->rv = fork_mid(f_info->fd[i - 1], f_info->fd[i], &f_info->pid[i], &pipex->heap);
		if (f_info->rv == -1)
			return (-1);
		if (f_info->pid[i] == 0)
			break ;
		i++;
	}
	return (0);
}

int main(int argc, char **argv, char **envp)
{
//	t_heap		heap;
//	t_fork_info	fork_info;
	t_pipex		pipex;
	int			i;

	ft_bzero(&pipex, sizeof(pipex));
	pipex.process_amount = argc - 3;
	pipex.pipe_amount = argc - 4;
//	if (parse_input(argc, argv, envp) == -1)
//		return (1);
	if (create_splits(argv, envp, &pipex.heap.splits, &pipex.heap.commands) != 0)
		return (printf("mis_split!\n"));

	if (create_pipes(&pipex.fork_info.fd, PIPE_AMOUNT) == -1)
		return (2);
	if (create_forks(&pipex) == -1)
		return (3);
	i = 0;
	while (i < pipex.process_amount && pipex.fork_info.pid[i] != 0)
		i++;
	if (i == pipex.process_amount)
	{
		printf("last child's own pid (from parent :P)->%d\n", pipex.fork_info.last_child);
//		 system("lsof -F cft0 -c pipex");
	}
	// wait_and_close_lop(&pid, argc);
	return (0);
}