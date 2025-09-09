/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:14:29 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/07 18:17:31 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXING_H
# define LEXING_H
# include "types.h"
# include "libft.h"
# include "list.h"

int		handle_operator(t_token **token_lst, char *input);
int		handle_word(t_token **token_lst, char *input);
t_token	*input_lexing(char *input, int add_end_node);

#endif
