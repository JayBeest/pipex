/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 22:14:24 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/01 13:15:08 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <pipex.h>
#include <utils.h>

#include <debug.h>

t_err	fork_end(int fd0[2], int fd1[2], t_cmd_info *cmd_info, t_fork_info *f_info)
{
	int	fd;

	f_info->pid = fork();
	if (f_info->pid == -1)
		return (FORK_FAIL);
	if (f_info->pid == 0)
	{
		if (dup2(fd0[0], STDIN_FILENO) == -1)
			return (DUP2_FAIL);
		close_pipe(fd0);
		if (f_info->access_outfile && fd1)
		{
			fd = open(f_info->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd == -1)
				return (OPEN_FAIL);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (DUP2_FAIL);
			close(fd);
		}
		if (cmd_info->cmd_not_found)
			return (NO_CMD);
		execv(cmd_info->full_cmd, cmd_info->cmd_split);
		return (EXECV_FAIL);
	}
	else
		close_pipe(fd0);
	return (NO_ERROR);
}

t_err	fork_mid(int fd0[2], int fd1[2], t_cmd_info *cmd_info, t_fork_info *f_info)
{
	f_info->pid = fork();
	if (f_info->pid == -1)
		return (FORK_FAIL);
	if (f_info->pid == 0)
	{
		if (dup2(fd0[0], STDIN_FILENO) == -1)
			return (DUP2_FAIL);
		close_pipe(fd0);
		if (dup2(fd1[1], STDOUT_FILENO) == -1)
			return (DUP2_FAIL);
		close_pipe(fd1);
		if (cmd_info->cmd_not_found)
			return (NO_CMD);
		execv(cmd_info->full_cmd, cmd_info->cmd_split);
		return (EXECV_FAIL);
	}
	else
		close_pipe(fd0);
	return (0);
}

t_err	fork_start(int fd0[2], int fd1[2], t_cmd_info *cmd_info, t_fork_info *f_info)
{
	int	fd;

	f_info->pid = fork();
	(void)fd0;
	if (f_info->pid == -1)
		return (FORK_FAIL);
	if (f_info->pid == 0)
	{
		if (dup2(fd1[1], STDOUT_FILENO) == -1)
			return (DUP2_FAIL);
		close_pipe(fd1);
		if (f_info->access_infile)
		{
			fd = open(f_info->infile, O_RDONLY);
			if (fd == -1)
				return (OPEN_FAIL);
			if (dup2(fd, STDIN_FILENO) == -1)
				return (DUP2_FAIL);
			close(fd);
			if (cmd_info->cmd_not_found)
				return (NO_CMD);
			execv(cmd_info->full_cmd, cmd_info->cmd_split);
			return (EXECV_FAIL);
		}
	}
	return (NO_ERROR);
}

t_err	setup_pipes_fd(t_fork_info *f_info, int child_amount, int index)
{
	if (index == 0)
	{
		f_info->type = START;
		if (pipe(f_info->fd1) == -1)
			return (PIPE_FAIL);
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
			return (PIPE_FAIL);
	}
	return (0);
}

t_err	create_forks(t_pipex *pipex)
{
	int						i;
	t_err					err;
	t_fork_info				*f_info;
	const static t_fork_fun	fun_ptr[3] = {
		[START] = fork_start,
		[MID] = fork_mid,
		[END] = fork_end
	};

	f_info = &pipex->fork_info;
	i = 0;
	while (i < pipex->child_amount)
	{
		setup_pipes_fd(f_info, pipex->child_amount, i);
		err = fun_ptr[f_info->type](f_info->fd0, f_info->fd1, \
				&pipex->cmd_info[i], f_info);
		if (err > NO_ERROR)
			return (err);
		if (f_info->pid == 0)
			break ;
		i++;
	}
	return (NO_ERROR);
}
