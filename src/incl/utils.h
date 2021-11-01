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

#ifndef UTILS_H
# define UTILS_H

void	ft_delstr(char *str);
void	close_pipe(int fd[2]);
void	free_heap(t_heap *heap);
int		wait_for_children(t_pipex *pipex);

#endif
