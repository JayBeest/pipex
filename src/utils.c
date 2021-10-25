/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/26 01:03:43 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <libft.h>
#include <pipex.h>

#include <debug.h>

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
	while (heap->command[i] && i < MAX_COMMANDS)
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
	while (heap->splits.cmd_split[i] && i < MAX_COMMANDS)
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

void	wait_and_free(t_pipex *pipex)
{
	int i;

	i = 0;
	printf("pipe_amount=%d\n", pipex->pipe_amount);
	printf("process_amount=%d\n", pipex->process_amount);
	while (i < pipex->process_amount)
	{
		int wait_rv;
		wait_rv = wait(NULL);
		printf("wait_rv=%d\n",wait_rv);
		i++;
	}
	system("lsof -F cft0 -c pipex");
	char *s;
	s = malloc(100);
	sprintf(s,"leaks --fullContent %d", getpid());
	system(s);
	free_heap(&pipex->heap);
	system(s);
}