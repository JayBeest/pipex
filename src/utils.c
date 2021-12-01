/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/12/01 01:48:54 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <pipex.h>

#include <stdio.h>

void	close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

void	clean_exit(int exit_code)
{
	unlink(".here_doc");
	exit (exit_code);
}

void	free_heap(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->child_amount)
	{
		ft_free_str(&pipex->cmd_info[i].full_cmd);
		ft_free_split(&pipex->cmd_info[i].cmd_split);
		i++;
	}
	ft_free_split(&pipex->fork_info.path_split);
	ft_free_str(&pipex->fork_info.infile);
	ft_free_str(&pipex->fork_info.outfile);
	ft_free_str(&pipex->fork_info.delimiter);
}

int	wait_for_children(t_pipex *pipex)
{
	int	status;
	int	last_status;
	int	i;

	i = 0;
	while (i < pipex->child_amount)
	{
		if (wait(&status) == pipex->fork_info.last_pid)
			last_status = status;
		i++;
	}
	if (pipex->fork_info.access_outfile == FALSE)
		return (NO_OUTFILE);
	if (pipex->cmd_info[i - 1].cmd_not_found)
		return (CMD_NOT_FOUND);
	else if (pipex->cmd_info[i - 1].permission_denied)
		return (CMD_NO_ACCESS);
	return (WEXITSTATUS(last_status));
}
