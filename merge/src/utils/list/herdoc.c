/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 10:57:37 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/12 03:31:43 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

t_hd	*create_hd(int fd)
{
	t_hd	*new_hd;

	new_hd = (t_hd *)malloc(sizeof(t_hd));
	if (!new_hd)
	{
		perror("malloc fail : create_hd");
		return (NULL);
	}
	new_hd->fd = fd;
	new_hd->next = NULL;
	return (new_hd);
}

t_hd	*find_last_hd(t_hd *hd_list)
{
	if (!hd_list)
		return (NULL);
	while (hd_list->next)
		hd_list = hd_list->next;
	return (hd_list);
}

void	addback_hd(t_hd **hd_list, t_hd *new)
{
	t_hd	*last_hd;

	if (!hd_list || !new)
		return ;
	if (*hd_list == NULL)
	{
		*hd_list = new;
		return ;
	}
	last_hd = find_last_hd(*hd_list);
	last_hd->next = new;
}

void	clear_hd_list(t_hd **hd_list)
{
	t_hd	*curr;
	t_hd	*next;

	if (!hd_list || !*hd_list)
		return ;
	curr = *hd_list;
	while (curr)
	{
		next = curr->next;
		close(curr->fd);
		free(curr);
		curr = NULL;
		curr = next;
	}
	*hd_list = NULL;
}

int	add_new_hd(t_hd **hd_lst, int fd)
{
	t_hd	*new_hd;

	new_hd = create_hd(fd);
	if (!new_hd)
		return (0);
	addback_hd(hd_lst, new_hd);
	return (1);
}
