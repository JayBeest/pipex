/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/01 13:13:20 by jcorneli         ###   ########.fr       */
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

void	free_strings(t_heap *heap)
{
	int	i;

	i = 0;
	while (heap->command[i])
	{
		free(heap->command[i]);
		heap->command[i] = NULL;
		i++;
	}
	ft_delstr(heap->infile);
	ft_delstr(heap->outfile);
}

void	free_heap(t_heap *heap)
{
	int	i;

	i = 0;
	while (heap->splits.cmd_split[i])
	{
		ft_free_split(heap->splits.cmd_split[i]);
		heap->splits.cmd_split[i] = NULL;
		i++;
	}
	if (heap->splits.path_split)
	{
		ft_free_split(heap->splits.path_split);
		heap->splits.path_split = NULL;
	}
	free_strings(heap);
}

int	wait_for_children(t_pipex *pipex)
{
	int	status;
	int	i;

	i = 0;
	while (i < pipex->child_amount)
	{
		wait(&status);
		if (WIFEXITED(status))
			printf("exit_code=%d\n", WEXITSTATUS(status));
		i++;
	}
	return (WEXITSTATUS(status));
}
