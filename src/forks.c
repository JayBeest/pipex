/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 22:14:24 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/26 22:14:24 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <pipex.h>
#include <utils.h>

#include <debug.h>

int	fork_end(int fd0[2], int fd1[2], t_fork_info *f_info, t_heap *heap)
{
	int	fd;

	f_info->pid = fork();
	if (f_info->pid == -1)
		return (-1);
	if (f_info->pid == 0)
	{
		printf("end_child     %d here->fd0[0] = %2d fd0[1] = %2d  fd1[0] = %2d fd1[1] = %2d\n", getpid(), fd0[0], fd0[1], fd1[0], fd1[1]);
		close(fd0[1]);
		if (dup2(fd0[0], STDIN_FILENO) == -1)
			return (1);
		close(fd0[0]);
		fd = open(heap->outfile, O_CREAT | O_WRONLY, 0666);
		if (fd == -1)
			return (2);
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (3);
		close(fd);
		execv(heap->command[f_info->i], heap->splits.cmd_split[f_info->i]);
	}
	else
		close_pipe(fd0);
	return (0);
}

int	fork_mid(int fd0[2], int fd1[2], t_fork_info *f_info, t_heap *heap)
{
	f_info->pid = fork();
	if (f_info->pid == -1)
		return (-1);
	if (f_info->pid == 0)
	{
		printf("mid_child     %d here->fd0[0] = %2d fd0[1] = %2d  fd1[0] = %2d fd1[1] = %2d\n", getpid(), fd0[0], fd0[1], fd1[0], fd1[1]);
		close(fd0[1]);
		if (dup2(fd0[0], STDIN_FILENO) == -1)
			return (1);
		close(fd0[0]);
		close(fd1[0]);
		if (dup2(fd1[1], STDOUT_FILENO) == -1)
			return (printf("dup2failinmid\n"));
		close(fd1[1]);
		execv(heap->command[f_info->i], heap->splits.cmd_split[f_info->i]);
	}
	else
		close_pipe(fd0);
	return (0);
}

int	fork_start(int fd0[2], int fd1[2], t_fork_info *f_info, t_heap *heap)
{
	int	fd;

	f_info->pid = fork();
	if (f_info->pid == -1)
		return (-1);
	if (f_info->pid == 0)
	{
		printf("start_child   %d here->fd0[0] = %2d fd0[1] = %2d  fd1[0] = %2d fd1[1] = %2d\n", getpid(), fd0[0], fd0[1], fd1[0], fd1[1]);
		close(fd1[0]);
		if (dup2(fd1[1], STDOUT_FILENO) == -1)
			return (1);
		close(fd1[1]);
		fd = open(heap->infile, O_RDONLY);
		if (fd == -1)
			return (2);
		if (dup2(fd, STDIN_FILENO) == -1)
			return (3);
		close(fd);
		execv(heap->command[f_info->i], heap->splits.cmd_split[f_info->i]);
	}
	return (0);
}

int	setup_pipes_fds(t_fork_info *f_info, int child_amount, int index)
{
	if (index == 0)
	{
		f_info->type = START;
		if (pipe(f_info->fd1) == -1)
			return (-1);
	}
	else if (index == child_amount - 1)
	{
		f_info->type = END;
		f_info->fd0[0] = f_info->fd1[0];
		f_info->fd0[1] = f_info->fd1[1];
	}
	else
	{
		f_info->type = MID;
		f_info->fd0[0] = f_info->fd1[0];
		f_info->fd0[1] = f_info->fd1[1];
		if (pipe(f_info->fd1) == -1)
			return (-2);
	}
	return (0);
}

int	create_forks(t_pipex *pipex)
{
	int						*i;
	t_fork_info				*f_info;
	const static t_fork_fun	fun_ptr[3] = {
		[START] = fork_start,
		[MID] = fork_mid,
		[END] = fork_end
	};

	f_info = &pipex->fork_info;
	i = &f_info->i;
	while (*i < pipex->child_amount)
	{
		setup_pipes_fds(f_info, pipex->child_amount, *i);
		if (fun_ptr[f_info->type](f_info->fd0, f_info->fd1, f_info, &pipex->heap) != 0)
			return (printf("forkerror!?!?!?!?!?!\n"));
		if (f_info->pid == 0)
			break ;
		*i = *i + 1;
	}
	return (0);
}
