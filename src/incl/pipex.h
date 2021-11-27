/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 09:35:42 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/24 23:42:40 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define OK 0
# define CMD_NO_ACCESS 126
# define CMD_NOT_FOUND 127
# define MAX_COMMANDS 1024

# include <libft.h>

typedef enum e_err
{
	NO_ERROR,
	NO_ACCESS,
	NO_CMD,
	NO_ARGS,
	MALLOC_FAIL,
	PIPE_FAIL,
	FORK_FAIL,
	DUP2_FAIL,
	OPEN_FAIL,
	EXECV_FAIL,
	ENVP_FAIL,
	DEL_FAIL,
	HERE_DOC_FAIL,
	DEFAULT
}				t_err;

typedef enum e_fork_type
{
	START,
	MID,
	END
}				t_fork_type;

typedef struct s_cmd_info
{
	t_bool	cmd_not_found;
	t_bool	permission_denied;
	char	*full_cmd;
	char	**cmd_split;
}				t_cmd_info;

typedef struct s_fork_info
{
	t_fork_type	type;
	t_bool		here_doc;
	t_bool		access_infile;
	t_bool		access_outfile;
	char		**path_split;
	char		*infile;
	char		*outfile;
	char		*delimiter;
	int			here_doc_fd;
	int			fd0[2];
	int			fd1[2];
	int			pid;
}				t_fork_info;

typedef struct s_pipex
{
	t_fork_info		fork_info;
	t_cmd_info		cmd_info[MAX_COMMANDS];
	int				child_amount;
}				t_pipex;

typedef t_err(*t_fork_fun)(t_cmd_info*, t_fork_info*);

t_err	create_errno_string(t_err error, char *str);

#endif