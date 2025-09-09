/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:34:37 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/07 18:29:10 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H
# include "types.h"
# include <stdlib.h>
# include <stdio.h>

//envp list fuction
t_envp	*create_envp(char *title, char *value);
t_envp	*find_last_envp(t_envp *envp_list);
void	addback_envp(t_envp **envp_list, t_envp *new);
void	clear_envp_list(t_envp **envp_list);

//token list fuction
t_token	*create_token(char *value, t_token_type type);
t_token	*find_last_token(t_token *token_list);
void	addback_token(t_token **token_list, t_token *new);
void	clear_token_list(t_token **token_list);
int		add_new_token(t_token **token_lst, char *value, t_token_type type);

//herdoc list fuction
t_hd	*create_hd(int fd);
t_hd	*find_last_hd(t_hd *hd_list);
void	addback_hd(t_hd **hd_list, t_hd *new);
void	clear_hd_list(t_hd **hd_list);
int		add_new_hd(t_hd **hd_lst, int fd);

#endif
