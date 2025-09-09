/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_herdoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:02:56 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/09 16:42:26 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "print.h"

void    print_herdocs(t_hd *hd_lst)
{
	char	buf[1024];
	ssize_t	n;

	if (hd_lst)
	{
		printf("***********HEREDOC LIST**********\n");
		while (hd_lst)
		{
			printf("[heredoc fd: %d]\n", hd_lst->fd);
			while ((n = read(hd_lst->fd, buf, sizeof(buf) - 1)) > 0)
			{
				buf[n] = '\0';
				printf("%s", buf);
			}
			printf("\n----------------------------\n");
			hd_lst = hd_lst->next;
		}
	}
}
