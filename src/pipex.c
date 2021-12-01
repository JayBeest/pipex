/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/12/01 01:34:19 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <pipex.h>
#include <parser.h>
#include <forks.h>
#include <utils.h>

#include <stdlib.h>

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	t_err	return_value;
	int		exit_code;

	if (argc != 5)
		return (1);
	ft_bzero(&pipex, sizeof(pipex));
	return_value = parse_input(argc, argv, envp, &pipex);
	if (return_value == MALLOC_FAIL)
		clean_exit(-1);
	if (return_value > MALLOC_FAIL)
		clean_exit(-2);
	if (pipex.fork_info.here_doc)
		exit (-3);
	return_value = create_forks(&pipex);
	if (return_value > MALLOC_FAIL)
		clean_exit(-4);
	if (pipex.fork_info.pid == 0)
		return (0);
	exit_code = wait_for_children(&pipex);
	free_heap(&pipex);
	unlink(".here_doc");
	return (exit_code);
}
