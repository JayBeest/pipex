/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/26 22:14:09 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pipex.h>
#include <unistd.h>

void	print_split(char **split)
{
	int	i;

	i = 0;
	printf("\n******\n\n");
	while (split[i])
	{
		printf("split[%d] = %s\n", i, split[i]);
		i++;
	}
	printf("\n******\n\n");
}

void	check_access(char *file)
{
	if (access(file, R_OK) == 0)
		printf("R_OK\n");
	if (access(file, W_OK) == 0)
		printf("W_OK\n");
	if (access(file, X_OK) == 0)
		printf("X_OK\n");
	if (access(file, F_OK) == 0)
		printf("F_OK\n");
	if (access(file, F_OK | R_OK) == 0)
		printf("F_OK | R_OK\n");
}

void	print_heap_index(t_heap *heap, int index)
{
	printf("*** PRINTING HEAP FOR INDEX: %d ***\n", index);
	printf("Command = |%s|\n", heap->command[index]);
	print_split(heap->splits.cmd_split[index]);
}
