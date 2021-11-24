/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:31:07 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/24 13:31:07 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <pipex.h>

t_err	print_custom_error(t_err error, char *err_str)
{
	(void)err_str;
	return (error);
}

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
