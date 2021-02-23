#pragma once
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>
#include <err.h>
#include <stdarg.h>
#include <ctype.h>

#include "../libmx/inc/libmx.h"             

typedef struct s_jobs {
    int num;
    int index;
    char sign;
    pid_t pid;
    char **data;
    char *pwd;
    struct s_jobs *next;
}              t_jobs;

typedef struct s_cd {
    int flag_num;
    int error;
    int flag_s;
    int flag_P;
    char *newpwd;
}              t_cd;

typedef struct s_history {
    void *data;
    struct s_history *next;
    struct s_history *prev;
}              t_history;

typedef struct s_env {
    char **n;
    int flag_i;
    int flag_u;
    char *clear[1];
}              t_env;

typedef struct s_ush {
    int exit;
    char *pwd;
    char *home;
    int last_return;
    struct s_list *pids;
    struct s_jobs *jobs;
    struct s_list *env_set;
    struct s_history *hist;
    struct termios savetty;
}              t_ush;


#define MX_ISLNK(m)      (((m) & 0170000) == 0120000)     

#define MX_FUNC_SYMBOLS "\"\'$`(){}\\;"
#define MX_SLASH_SPEC_DBLQ "`$\"\\"
#define MX_SLASH_SPEC "`$\"\\ '(){};~"

#define MX_ECHO_ESC_SPEC_CHAR "\a\b\x1b\f\n\r\t\v\\"
#define MX_ECHO_LITERAL_SPEC_CHAR "abefnrtv\\"

#define MX_IS_SP_TAB_NL(c) (c == '\n' || c == '\t' || c == ' ')

enum e_quote {
    SIN_Q,
    DBL_Q,
    TDBL_Q,  
    BCK_Q,
    TBCK_Q,  
    DOL_CMD,
    TDOL_CMD,  
    DOL_P,
    DOL_BP,
    SLASH,
    TSLASH,  
    SEMICOL,
    OUT_CMDS,  
    OUT_DBQ,  
    OUT_SUB,
    NUM_Q
};

enum e_spec_ch_mark {
    M_SKP = -10,  
    M_SKSL, 
    M_DEL,  
    M_SEMIC, 
    NUM_MC = 4
};

enum e_echo_flags {
    ECHO_NONL,
    ECHO_SPEC,
    ECHO_NOSPEC,
    NUM_ECHO_FLAGS
};

typedef struct s_quotes_params_data {
    int start;
    int end;
}              t_range;

typedef struct s_formatting_list {
    struct s_quotes_params_data *data;
    struct s_formatting_list *next;
}              t_frmt_lst;


/*  detect_builds.c  */
int mx_detect_builds(char **args, t_ush *ush);

/*  detect_exp.c  */
int mx_detect_exp(char **proc, t_history *start_h, t_list **env_set);

/*  env_in_list.c  */
bool mx_env_in_list(t_list **env_set, char *src);

/*  format_funcs.c  */
void mx_pop_format(t_frmt_lst **del);
void mx_push_format(t_frmt_lst **add, int start, int end, t_frmt_lst **del);
void mx_push_back_format(t_frmt_lst **add, int start, int end, t_frmt_lst **del);
void mx_free_format_lists(t_frmt_lst **arr);

/*  history_list.c  */
void mx_push_front_history(t_history **hs, char *data);
void mx_free_node(t_history *node);
void mx_free_history(t_history **list);

/*  job_add.c  */
void mx_add_job(t_jobs **j, char **args, pid_t pid);

/*  job_del.c  */
void mx_del_job(t_jobs **jobs, int flag, t_jobs **first);

/*  job_funcs.c  */
bool mx_job_num_find(char *args, t_jobs **jobs);
bool mx_job_chars_find(char *args, t_jobs **jobs);
char *mx_cut_str_forjob(char *args);
t_jobs *mx_create_job(char **data, int num, pid_t pid, char *pwd);
void mx_free_jobs(t_jobs **jobs);

/*  list_funcs.c  */
void mx_push_back_list(t_list **list, void *data);
void mx_pop_front_list(t_list **head);
void mx_free_list(t_list **list);

/*  parse.c  */
void mx_replace_sub_str(char **s, int start, int end, char *replace);
void mx_quit_parse(char *line, t_ush *ush, int ret_val, t_frmt_lst **arr );
int mx_parse_exec(char *subline, t_ush *ush);
int mx_semicolon_split(char *line, t_ush *ush, char ***subcommands);
bool mx_check_parse(char *str);
int mx_parse(char *line, t_ush *ush);

/*  parse_get_format_str.c  */
int mx_get_format_str(char *s, t_frmt_lst **arr);

/*  parse_get_process_output.c  */
char *mx_get_subst_outputs(char *str, t_ush *ush);

/*  parse_get_substr_replace_str.c  */
char *mx_get_subst_replace_str(char **str, t_frmt_lst *list, t_ush *ush);

/*  parse_mark_chars.c  */
t_range *mx_is_inside_of(int i, enum e_quote type, t_frmt_lst **arr);
void mx_mark_semicolon(char *s, t_frmt_lst **arr);
void mx_mark_quotes(char *s, t_frmt_lst **arr);
void mx_mark_chars(char *s, t_frmt_lst **arr);
char *mx_clear_str(char *str);

/*  parse_outer_substr.c  */
void mx_create_outer_substr(char *s, t_frmt_lst **arr);

/*  parse_substitutions.c  */
int mx_handle_substitutions(char **str, t_frmt_lst **arr, t_ush *ush);

/*  parse_symbols.c  */
int mx_check_double_quote(char *s, int *i, t_frmt_lst **arr);
int mx_check_open_paren(char *s, int *i, t_frmt_lst **arr);
int mx_check_close_paren(char *s, int *i, t_frmt_lst **arr);
int mx_check_open_brace(char *s, int *i, t_frmt_lst **arr);
int mx_check_close_brace(char *s, int *i, t_frmt_lst **arr);
int mx_check_dollar(char *s, int *i, t_frmt_lst **arr);
int mx_tilde_expansion(char **argv, t_ush *ush);
int mx_check_backquote(char *s, int *i, t_frmt_lst **arr);
int mx_check_single_quote(char *s, int *i, t_frmt_lst **arr);

/*  signal_funcs.c  */
void mx_signal_reload();
void mx_signal_ignore();
void mx_signal_def();
void mx_init_signals();

/*  status_proc.c  */
int mx_status_proc(char **args, t_jobs **jobs);

/*  stream_funcs.c  */
char *mx_stream(int buf, char *line, int *x);
char *mx_read_stream(t_ush *ush, t_history *h);

/*  ush_cd.c  */
char *mx_create_newpwd(char *newpwd, char **m, t_cd *in);
int mx_ush_cd(char **args, t_ush *ush);

/*  ush_echo.c  */
int mx_ush_echo(char **argv);

/*  ush_env.c  */
int mx_ush_env(char **args, t_jobs **jobs);
int mx_env_print(void);

/*  ush_exit.c  */
int mx_ush_exit(char **args, t_ush *ush);

/*  ush_export.c  */
int mx_ush_export(char **args, t_list **env_set);

/*  ush_fg.c  */
int mx_ush_fg(char **args, t_jobs **jobs);

/*  ush_jobs.c  */
int mx_ush_jobs(char **args, t_jobs **jobs);

/*  ush_pwd.c  */
int mx_ush_pwd(char **args, t_ush *ush);

/*  ush_unset.c  */
int mx_ush_unset(char **args, t_list **env_set);

/*  ush_which.c  */
int mx_ush_which(char **args);

/*  utility_funcs.c  */
char *mx_strpart(char *str, int index);
int mx_no_such(char *name, char *args, char *str);
int mx_not_found(char *args, char *str);
int mx_name_search(char *tmp , t_jobs *jobs);
bool mx_opencheck(char *dirname, int *error, int flag_link);
char **mx_copy_dub_arr(char **args);
struct termios *mx_get_tty(void);
void mx_enable_canonic_mode(void);
void mx_disable_canonic_mode(void);
void mx_setup_term(t_ush *ush);
char mx_getopt(int argc, char **argv, char *optstring, int *optind);
