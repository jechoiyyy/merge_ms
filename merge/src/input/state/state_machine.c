/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsagong <dsagong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:26:18 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/05 17:24:55 by dsagong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

t_quote_state	state_machine(char c, t_quote_state state)
{
	if (state == STATE_GENERAL)
	{
		if (c == '\'')
			return (STATE_IN_SQUOTE);
		if (c == '\"')
			return (STATE_IN_DQUOTE);
		return (STATE_GENERAL);
	}
	else if (state == STATE_IN_SQUOTE && c == '\'')
		return (STATE_GENERAL);
	else if (state == STATE_IN_DQUOTE && c == '\"')
		return (STATE_GENERAL);
	return (state);
}
