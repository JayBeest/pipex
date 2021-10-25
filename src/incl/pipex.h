/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/20 01:37:31 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define MAX_COMMANDS 512

#include <stdio.h>


typedef enum	e_bool
{
	FALSE,
	TRUE
}				t_bool;

typedef enum	e_errno
{
	ARG_COUNT,
	FORK
}				t_errno;

typedef struct	s_splits
{
	char **cmd_split[MAX_COMMANDS];
	char **path_split;
}				t_splits;

typedef struct s_heap
{
	int			i;
	t_splits	splits;
	char		*command[MAX_COMMANDS];
}				t_heap;

// typedef struct s_arguments
// {
// 	int		argc;
// 	char	**argv;
// 	char	**envp;
// }				t_arguments;

typedef struct s_fork_info
{
	int	rv;
	// int last_child_id;
	int	pid[MAX_COMMANDS];
	int	fd[MAX_COMMANDS][2];
	int	duped_fd[MAX_COMMANDS][2];
}				t_fork_info;

typedef struct s_pipex
{
	int 		process_amount;
	int 		pipe_amount;
	t_heap		heap;
	t_fork_info	fork_info;

}				t_pipex;
#endif