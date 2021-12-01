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
	printf("split[%d] = %s\n", i, split[i]);
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

void	debug(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->child_amount)
	{
		if (pipex->cmd_info[i].cmd_split)
			print_split(pipex->cmd_info[i].cmd_split);
		else
			printf("cmd_info[%d].cmd_split == NULL\n", i);
		if (pipex->cmd_info[i].full_cmd)
			printf("full_cmd[%d]->%s\n", i, pipex->cmd_info[i].full_cmd);
		else
			printf("cmd_info[%d].full_cmd == NULL\n", i);
		i++;
	}
}
