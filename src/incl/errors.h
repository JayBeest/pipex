/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorneli <marvin@codam.nl>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:31:41 by jcorneli          #+#    #+#             */
/*   Updated: 2021/11/24 13:31:41 by jcorneli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

t_err	print_custom_error(t_err error, char *err_str);
t_err	print_errno_string(t_err error, char *err_str);
t_err	create_errno_string(t_err error, char *str);

#endif