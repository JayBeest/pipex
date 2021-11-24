/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dupstd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:49:18 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/24 13:49:18 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <pipex.h>

t_err	open_to_stdout(t_fork_info f_info)
{
	int	fd;

	if (f_info.here_doc)
		fd = open(f_info.outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(f_info.outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (OPEN_FAIL);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (DUP2_FAIL);
	}
	close(fd);
	return (NO_ERROR);
}

t_err	open_to_stdin(t_fork_info f_info)
{
	int	fd;

	if (f_info.here_doc)
		fd = f_info.here_doc_fd;
	else
		fd = open(f_info.infile, O_RDONLY);
	if (fd == -1)
		return (OPEN_FAIL);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (DUP2_FAIL);
	}
	close(fd);
	return (NO_ERROR);
}
