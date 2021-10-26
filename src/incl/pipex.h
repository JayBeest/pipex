/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/26 00:05:25 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <debug.h>
# include <stdio.h>

#ifndef PIPEX_H
# define PIPEX_H

# define OK 0
# define MAX_COMMANDS 1024

typedef enum e_fork_type
{
	START,
	MID,
	END
}				t_fork_type;

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
	t_splits	splits;
	char		*command[MAX_COMMANDS];
	char 		*infile;
	char 		*outfile;
}				t_heap;

typedef struct s_fork_info
{
	t_fork_type	type;
	int			pid;
	int			fd0[2];
	int 		fd1[2];
	int			i;
}				t_fork_info;

typedef struct s_pipex
{
	int 		child_amount;
	int 		pipe_amount;
	t_heap		heap;
	t_fork_info	fork_info;

}				t_pipex;

typedef int(*t_fork_fun)(int[2], int[2], t_fork_info*, t_heap*);

#endif