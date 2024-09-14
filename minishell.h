#ifndef MINISHELL_H
#define MINISHELL_H


#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>


#define string 1
#define operator 2
#define Pipe 5
#define redir_input 6
#define heredoc 7
#define redir_out_overwrite 8
#define redir_out_append 9
#define dollar_sign 10
//-------------------------------------------
#define command 21
#define arguments 22
#define infile 23
#define outfile_overwrite 24
#define outfile_append 25
#define heredoc_lim 26
//-----------------------------------------
#define s_command 31

#define heredoc_cmd "haha_heredoc_XD"

typedef struct s_lexing
{
	char *line;
	char **res;
	int start;
	int i;
	char **envp;
	int j;
	int	exit_code;
}	t_lexing;

typedef struct s_token
{
    char *token;
    int type;

    struct s_token *next;
    struct s_token *prev;
} t_tokens;

typedef struct s_simple_command
{
	char *cmd;
	char **array;
} t_simple_command;

typedef struct s_redir_out
{
	char *outfile;
	int fd;
} t_redir_out;

typedef struct s_redir_in
{
	char	*in_file;
	int		fd;
	int		here_doc;
} t_redir_in;

typedef struct s_pipe
{
	int pipe_fd[2];
} t_pipe;

typedef struct s_heredoc
{
	char	*limiter;
} t_heredoc;

typedef struct s_node
{
	int					type;
	t_simple_command	simple_cmd;
	t_redir_in			redir_in;
	t_redir_out			redir_out;
	t_pipe				piping;
	t_heredoc			heredoc_obj;

	struct s_node *next;
	struct s_node *prev;
}	t_node;

typedef struct s_data
{
	char	**envp;
	char	*input_line;
	char	**tokens;
	int		saved_in_fd;
	int		saved_out_fd;
	int		exit_code;
	t_node	*instr_list;
}	t_data;

typedef struct s_cmd_list
{
	char **cmd;
	int in_fd;
	int	out_fd;
	pid_t	pid;

	struct s_cmd_list *next;
	struct s_cmd_list *prev;
}	t_cmd_list;

extern void rl_replace_line(const char *, int);
char **lexer(char *line, t_data *data);

//expansion
void	expansion(t_tokens *list, t_data *data);

//expansion2
int	copy_substr(char **res, char *substr, int i);
char	*get_expand_string(char *str, char **envp);
char	*get_substr(char *line, int i);

//utils
void	free_2d_array(char **str);
void	print_2d_array(char **cmd);
int		exit_process(char **arg, t_data *data);
int		set_terminos_echo(int enable);
void	increment_shell_lvl(t_data *data);
void	change_env_oldpwd(t_data *data);
void	change_env_pwd(t_data *data);
void	free_t_tokens(t_tokens *obj);
void	free_t_node(t_node *obj);
void	free_t_cmd_list(t_cmd_list *obj);
int		waitpid_and_get_exit_status(pid_t pid);

//execve
char	*get_path(char **envp, char *cmd);
int		execute(char **cmd, t_data **data, t_cmd_list *templist);

//lexer2
int handle_quotes(char *line, int i);
int	lexer_helper1(t_lexing *lexer);
int	lexer_helper2(t_lexing *lexer);
int lexer_helper3(t_lexing *lexer);
int	init_lexer_struct(t_lexing *lexer, t_data *data, char *line);
int	count_lexer_array(char *line);

//lexer3
void	tokens_list_processing(t_tokens *list);
void	remove_quotes_from_token_list(t_tokens *list);
char	**semi_ft_split(char *line);

//lexer4
int	check_valid_list(t_tokens *list);
t_tokens	*get_last_node(t_tokens *tokens);

//parse_list
t_node	*make_final_list(t_tokens *tokens);
t_node	*make_final_list_heredoc(t_node *list);

//pwd
int	pwd(void);

//echo
int	echo(char **cmd);

//env
int	env(t_data *data);

//cd
int cd(char **cmd, t_data *data);

void	print_token_array(char **str);
int check_operator(int n);
void	identify_tokens_list2(t_tokens *list);
void	identify_tokens_list(t_tokens *tokens);
t_tokens* init_token_list(t_data *data);
void print_tokens_list(t_tokens *list);
void	print_final_list(t_node *list);

//export
void	malloc_dup_env(char **dest, char **env);
int total_strings(char **env);
int export(char ***env, char **str);

//unset
int unset(char ***env, char **str);
void	print_env(char **env);
char **unset_array(char **env, char *str);
int	check_str(char *str);
int	print_sorted_env(char **tempenv);

//cmdlist
t_cmd_list *make_command_list(t_node *list);

#endif