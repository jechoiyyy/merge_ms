/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:45:02 by jechoi            #+#    #+#             */
/*   Updated: 2025/09/10 14:28:58 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "types.h"
# include "libft.h"
# include "utils.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

int		is_builtin_command(char *cmd);
int		ft_cd(t_cmd *cmd, t_shell *shell);
int		ft_echo(t_cmd *cmd);
int		ft_env(t_shell *shell);
int		ft_exit(t_cmd *cmd, t_shell *shell);
int		ft_export(t_cmd *cmd, t_shell *shell);
void	display_all_exports(t_shell *shell);
int	set_env_variable(t_shell *shell, char *key, char *value);
void	update_env_array(t_shell *shell);
int		ft_pwd(void);
int		ft_unset(t_cmd *cmd, t_shell *shell);

int		is_valid_identifier(char *name);
char	*extract_key2(char *arg);
char	*extract_value(char *arg);
t_envp	*find_env_node(t_envp *envp_list, char *key);
char	*get_path_env(t_shell *shell);
char	*find_executable(char *command, t_shell *shell);

#endif