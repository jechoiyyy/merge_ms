#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

# define SUCCESS 0
# define FAILURE 1

# define SIG_INTERACTIVE 1
# define SIG_NON_INTERACTIVE 0
# define SIG_CHILD 2

extern volatile sig_atomic_t g_signal_received;

typedef enum    e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_APPEND,
    TOKEN_HEREDOC,
    TOKEN_EOF
}   t_token_type;

typedef struct  s_env
{
    char    *key;
    char    *value;
    struct s_env    *next;
}   t_env;

typedef struct  s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;

typedef struct  s_redirect
{
    t_token_type    type;
    char            *file;
    struct s_redirect   *next;
}   t_redirect;

typedef struct  s_cmd
{
    char    **args; // token_lst를 파싱
    char    *input_file; // token_lst를 받아와서 파싱
    char    *output_file; // token_lst 파싱
    int     append_mode; // token_lst 파싱
    char    *heredoc_delimiter; // int fd로 받아오기 전환 / 
    // fd로 만들었으니까 따로 동작할 필요없고, fd를 stdin_fillno로 넣어서 사용
    struct s_cmd    *next;
}   t_cmd;


typedef struct  s_shell
{
    t_env   *env_list;
    char    **env_array;
    int     last_exit_status;
    int     exit_flag;
    int     signal_mode;
}   t_shell;

// minishell
int     main(int argc, char **argv, char **envp);
int     init_shell(t_shell *shell, char **envp);

// env, token
t_env   *create_env_node(char *env_str);
void    add_env_node(t_env **head, t_env *new_node);
void    free_env_list(t_env *head);
char    **env_list_to_array(t_env *env_list);
void    free_env_array(char **env_array);
void    print_error(char *cmd, char *msg);

char	*expand_variables(char *str, t_shell *shell);
char	*get_env_value(char *key, t_shell *shell);

// builtins
// executor

// Lexer
t_token	*tokenize(char *line, t_shell *shell);
t_token	*create_token(t_token_type type, char *value);
void	add_token(t_token **head, t_token *new_token);
void	free_tokens(t_token *head);
int	is_special_char(char c);
int	is_quote_char(char c);
void	skip_whitespace(char **line);
int handle_quotes(char **line, char **start);
char	*expand_variables(char *str, t_shell *shell);

// Parser
t_cmd	*create_command(void);
void add_argument(t_cmd *cmd, char *arg);
int	is_redirect_token(t_token_type type);
t_cmd	*parse_tokens(t_token *tokens);
t_cmd	*parse_pipeline(t_token **current);
t_cmd	*parse_simple_command(t_token **current);
int	parse_redirections(t_token **current, t_cmd *cmd);
void	set_input_file(t_cmd *cmd, char *file);
void	set_output_file(t_cmd *cmd, char *file, int append);
void	set_heredoc_delimiter(t_cmd *cmd, char *delimiter);
void	free_commands(t_cmd *commands);


// Signal
void	setup_signals(void);
void	signal_handler_interactive(int sig);
void	signal_handler_noninteractive(int sig);
void	setup_signals_interactive(void);
void	setup_signals_noninteractive(void);
void	setup_signals_child(void);
void	handle_signal_in_loop(t_shell *shell);

//utils
char	*ft_strncpy(char *dest, const char *src, size_t n);
char	*ft_strcpy(char *dest, const char *src);

void	print_tokens(t_token *tokens);
void	print_commands(t_cmd *commands);

// clean
void    cleanup_shell(t_shell *shell);

#endif