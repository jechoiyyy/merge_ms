/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:44:16 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/11 11:13:46 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SETUP_H
# define SETUP_H
# include "types.h"
# include "libft.h"

t_envp	*set_envp(char **envp);
int		ready_minishell(t_prompt *prompt, t_shell *shell, char **envp);

#endif
