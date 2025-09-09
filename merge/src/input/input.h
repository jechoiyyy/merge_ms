/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:42:31 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/07 20:09:09 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H
# include "types.h"

void				syntax_print(t_grammar_status status, t_token **token_lst);
t_grammar_status	grammar_check(t_prompt *prompt);
int					input_process(t_prompt *prompt);

#endif
