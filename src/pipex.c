/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/03 23:23:29 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <libft.h>
#include <pipex.h>
#include <parser.h>
#include <forks.h>
#include <utils.h>

#include <stdio.h>

t_err	print_errno_string(t_err error, char *err_str)
{
	if (error == NO_CMD)
	{
		err_str = ft_strjoin(err_str, ": command not found");
		if (!err_str)
			return (MALLOC_FAIL);
		ft_putendl_fd(err_str, 2);
		free(err_str);
	}
	else
		perror(err_str);
	return (error);
}

t_err	print_custom_error(t_err error, char *err_str)
{
	(void)err_str;
	return (error);
}

t_err	create_errno_string(t_err error, char *str)
{
	char	*temp_str;
	char	*err_str;
	t_err	print_err;

	if (error > NO_ERROR)
		temp_str = ft_strdup("pipex: ");
	else
		temp_str = ft_strdup(str);
	if (!temp_str)
		return (MALLOC_FAIL);
	err_str = ft_strjoin(temp_str, str);
	free(temp_str);
	if (!err_str)
		return (MALLOC_FAIL);
	if (error > NO_CMD)
		print_err = print_custom_error(error, err_str);
	else
		print_err = print_errno_string(error, err_str);
	free(err_str);
	if (print_err == MALLOC_FAIL)
		return (MALLOC_FAIL);
	return (error);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	t_err	return_value;
	int		exit_code;

//	pipex.fork_info.delimiter = ft_strdup("DELIMITER");
	ft_bzero(&pipex, sizeof(pipex));
	return_value = parse_input(argc, argv, envp, &pipex);
	if (return_value == MALLOC_FAIL)
	{
		free_heap(&pipex);
		return (-1);
	}
	return_value = create_forks(&pipex);
	if (return_value > MALLOC_FAIL)
		return (-2);
	if (pipex.fork_info.pid == 0)
		return (0);
	exit_code = wait_for_children(&pipex);
	free_heap(&pipex);
	return (exit_code);
}
