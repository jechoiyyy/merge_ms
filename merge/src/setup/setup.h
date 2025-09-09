/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:44:16 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/05 17:34:11 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SETUP_H
# define SETUP_H
# include "types.h"
# include "libft.h"

t_envp	*set_envp(char **envp);
int		ready_minishell(t_prompt *prompt, char **envp);

#endif
