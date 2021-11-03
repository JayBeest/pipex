/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/03 00:26:17 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <libft.h>
#include <pipex.h>

#define OK 0

void	close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

void	ft_delstr(char *str)
{
	if (!str)
		return ;
	free(str);
	str = NULL;
}

void	free_heap(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->child_amount)
	{
		ft_delstr(pipex->cmd_info[i].full_cmd);
		ft_free_split(pipex->cmd_info[i].cmd_split);
		i++;
	}
	ft_free_split(pipex->fork_info.path_split);
	ft_delstr(pipex->fork_info.infile);
	ft_delstr(pipex->fork_info.outfile);
}

int	wait_for_children(t_pipex *pipex)
{
	int	status;
	int	i;

	i = 0;
	while (i < pipex->child_amount)
	{
		wait(&status);
		i++;
	}
	if (pipex->cmd_info[i - 1].cmd_not_found)
		return (127);
	return (WEXITSTATUS(status));
}
