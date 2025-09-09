/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:13:51 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/08 14:28:13 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H
# include "types.h"
# include "utils.h"
# include <stdlib.h>
# include <stdio.h>

//exp.c
int				set_exp_data(t_exp *exp_data);
int				exp_normal_process(t_exp *exp_data, char *value);
int				exp_env_process(t_exp *exp_data, char *value, t_envp *envp_lst);

//expand.c
char			*expand_process(char *value, t_envp *envp_lst);
int				expand_token(t_token *token_lst, t_envp *envp_lst);

#endif
