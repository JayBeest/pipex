#include <unistd.h>
#include <fcntl.h>
#include <pipex.h>
#include <utils.h>

#include <debug.h>

static int	fork_start(int pipe_fd1[2], int *pid, t_heap *heap)
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
			fd = open(heap->infile, O_RDONLY);
			if (fd == -1)
				return (2);
			if (dup2(fd, STDIN_FILENO) == -1)
				return (3);
			close(fd);
			execv(heap->command[heap->i], heap->splits.cmd_split[heap->i]);
		}
	return (0);
}

static int	fork_end(int pipe_fd2[2], int *pid, t_heap *heap)
{
	int	fd;

	*pid = fork();
	if (*pid == -1)
		return (-1);
	if (*pid == 0)
		{
			printf("end_child   %d here->fd2[0] = %2d fd2[1] = %2d\n", getpid(), pipe_fd2[0], pipe_fd2[1]);
			close(pipe_fd2[1]);
			if (dup2(pipe_fd2[0], STDIN_FILENO) == -1)
				return (1);
			close(pipe_fd2[0]);
			fd = open(heap->outfile, O_CREAT | O_WRONLY, 0666);
			if (fd == -1)
				return (2);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (3);
			close(fd);
			execv(heap->command[heap->i], heap->splits.cmd_split[heap->i]);
		}
	else
		close_pipe(pipe_fd2);
	return (0);
}

static int	fork_mid(int pipe_fd1[2], int pipe_fd2[2], int *pid, t_heap *heap)
{
	*pid = fork();
	if (*pid == -1)
		return (-1);
	if (*pid == 0)
		{
			printf("mid_child   %d here->fd1[0] = %2d fd1[1] = %2d  fd2[0] = %2d fd2[1] = %2d\n", getpid(), pipe_fd1[0], pipe_fd1[1], pipe_fd2[0], pipe_fd2[1]);
			close(pipe_fd1[1]);
			if (dup2(pipe_fd1[0], STDIN_FILENO) == -1)
				return (1);
			close(pipe_fd1[0]);
			close(pipe_fd2[0]);
			if (dup2(pipe_fd2[1], STDOUT_FILENO) == -1)
				return (printf("dup2failinmid\n"));
			close(pipe_fd2[1]);
			execv(heap->command[heap->i], heap->splits.cmd_split[heap->i]);
		}
	else
		close_pipe(pipe_fd1);
	return (0);
}

int	create_forks(t_pipex *pipex)
{
	int			*i;
	t_fork_info	*f_info;

	f_info = &pipex->fork_info;
	i = &pipex->heap.i;
	while (*i < pipex->process_amount)
	{
		if (*i == 0)
			f_info->rv = fork_start(f_info->fd[*i], &f_info->pid[*i], &pipex->heap);
		else if (*i == pipex->process_amount - 1)
			f_info->rv = fork_end(f_info->fd[*i - 1], &f_info->pid[*i], &pipex->heap);
		else
			f_info->rv = fork_mid(f_info->fd[*i - 1], f_info->fd[*i], &f_info->pid[*i], &pipex->heap);
		if (f_info->rv != 0)
			return (printf("forkerror!?!?!?!?!?!\n"));
		if (f_info->pid[*i] == 0)
			break ;
		*i = *i + 1;
	}
	return (0);
}