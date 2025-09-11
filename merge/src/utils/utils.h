/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 09:51:37 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/12 01:28:22 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include "types.h"
# include "libft.h"

int		is_space(char c);
int		is_operator(char c);
int		is_quote(char c);
int		is_redir_type(t_token_type type);
int		is_valid_expand(t_exp *exp_data, char *value);

char	*charjoin_and_free(char *s, char c);
int	is_redirect_token(t_token_type type);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strncpy(char *dest, const char *src, size_t n);
char	*ft_strcat(char *dest, const char *src);

int init_shell(t_shell *shell);
t_envp	*create_env_node(char *env_str);
void	add_env_node(t_envp **head, t_envp *new_node);
char	**env_list_to_array(t_envp *envp_list);
void	free_env_list(t_envp *head);
void	free_env_array(char **env_array);
void	cleanup_shell(t_shell *shell);
void	print_error(char *cmd, char *msg);

#endif
