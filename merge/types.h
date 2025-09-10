/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jechoi <jechoi@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:41:47 by dsagong           #+#    #+#             */
/*   Updated: 2025/09/10 16:12:07 by jechoi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H
# include <unistd.h>

# define SUCCESS 0
# define FAILURE 1

# define SIG_INTERACTIVE 1
# define SIG_NON_INTERACTIVE 0
# define SIG_CHILD 2

typedef struct s_envp
{
	char			*key;
	char			*value;
	struct s_envp	*next;
}	t_envp;

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND,
	T_HEREDOC,
	T_ERROR,
	T_CORRECT_FILNAME,
	T_WRONG_FILNAME,
	T_END
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef enum e_quote_state
{
	STATE_GENERAL,
	STATE_IN_SQUOTE,
	STATE_IN_DQUOTE
}	t_quote_state;

typedef struct s_exp
{
	t_quote_state	state;
	size_t			idx;
	char			*result;
}	t_exp;

typedef enum e_grammar_status
{
	GRAMMAR_OK,
	GRAMMAR_TOKEN_ERROR,
	GRAMMAR_EOF_ERROR,
}	t_grammar_status;

typedef struct s_hd
{
	int			fd;
	struct s_hd	*next;
}	t_hd;

typedef struct s_prompt
{
	char		*input;
	t_envp		*envp_lst;
	t_token		*token_lst;
	t_hd		*hd_lst;
}	t_prompt;

typedef struct s_filename
{
    char    *filename;
    int     flag;
	struct s_filename	*next;
}   t_filename;

typedef struct  s_cmd
{
    char    **args;
    t_filename    *input_file;
    t_filename    *output_file;
    int     append_mode;
    int		hd;
    struct s_cmd    *next;
}   t_cmd;

typedef struct  s_shell
{
    t_envp   *envp_list;
    char    **env_array;
    int     last_exit_status;
    int     exit_flag;
    int     signal_mode;
}   t_shell;

#endif
