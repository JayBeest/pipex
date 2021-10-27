/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 22:14:40 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/27 02:18:54 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <pipex.h>

t_err	create_path_split(char **envp, char ***path_split)
{
	char	*curr_envp;
	int		i;

	i = 0;
	if (!envp)
		return (ENVP_FAIL);
	while (envp[i])
	{
		curr_envp = ft_strnstr(envp[i], "PATH=", 5);
		if (curr_envp)
			break ;
		i++;
	}
	*path_split = ft_split(curr_envp + 5, ':');
	if (!*path_split)
		return (MALLOC_FAIL);
	return (NO_ERROR);
}
