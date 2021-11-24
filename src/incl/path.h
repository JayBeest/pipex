/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 22:15:52 by jcorneli          #+#    #+#             */
/*   Updated: 2021/10/26 22:15:52 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_H
# define PATH_H

t_err	create_path_split(char **envp, char ***path_split);
t_err	correct_path(char *current_path, char *cmd, char **cmd_ptr);
t_err	check_set_cmd(char **path_split, t_cmd_info *cmd_info);

#endif