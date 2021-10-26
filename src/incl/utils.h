/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 14:53:02 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/15 15:50:36 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_UTILS_H
# define PIPEX_UTILS_H

void	close_pipe(int fd[2]);
void	wait_and_free(t_pipex *pipex);
int		create_path_split(char **envp, char ***path_split);
void	free_heap(t_heap *heap);
char	*find_and_set_path(char *cmd_ptr, char **envp, t_splits *splits);

#endif
