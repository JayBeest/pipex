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

# define INFILE argv[1]
# define CMD1 argv[2]
# define CMD2 argv[3]
# define OUTFILE argv[4]

# define ENVP_PATH_CODAM envp[6]
# define ENVP_PWD_CODAM envp[14]
# define ENVP_PATH_M1 envp[0]
# define ENVP_PWD_M1 envp[20]

# define PROCESS_AMOUNT argc - 3
# define PIPE_AMOUNT argc - 4

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
}				t_fork_info;

typedef struct s_pipex
{
	int 		process_amount;
	int 		pipe_amount;
	t_heap		heap;
	t_fork_info	fork_info;

}				t_pipex;
#endif