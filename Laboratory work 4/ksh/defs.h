#ifndef _DEFS_H_
#define _DEFS_H_    1

#include <unistd.h>
#include <sys/types.h>

#define UNUSED(x) ((void)(x))

typedef int bool_t;
#define true 1
#define false 0

typedef struct shell_settings_s {
    // Путь к файлу, из которого будут считаны команды
    char* path_to_source_file;
    // Команда, указанная после ключа `-c'
    char* command_to_execute;
    // Многословный режим
    bool_t verbose;
} shell_settings_t;

static const shell_settings_t DEFAULT_SHELL_SETTINGS = {
        .path_to_source_file = NULL,
        .command_to_execute = NULL,
        .verbose = false,
};

shell_settings_t shell_settings;

typedef struct cmd_s {
    int argc;
    char** argv;
    pid_t pid;
    int in;
    int out;
    bool_t is_piped;
    bool_t is_forward;
    bool_t is_thrown;
    bool_t is_appending;
    char* redirect_filename;
    bool_t is_fwd_in;
    char* name;
    bool_t is_fwd_to_stream;
    int stream_descriptor;
} cmd_t;

// Действие, которое должна выполнить оболочка по завершении команды.
typedef enum action_e {
    /**
     * Оболочка может продолжать работу.
     */
            CMD_ACT_CONTINUE,
    /**
     * Оболочка должна завершить работу.
     */
            CMD_ACT_EXIT,
} action_t;

/**
 * Результат исполнения команды.
 *
 * Содержит код возврата и используется вместо него,
 * чтобы выполнять над оболочкой некоторые действия
 * после завершения определённых команд.
 *
 * Так, например, exit возвращает структуру, в поле action
 * которой содержится значение CMD_ACT_EXIT, которое
 * предписывает оболочке завершить работу.
 */
typedef struct cmd_result_s {
    /**
     * Код возврата команды.
     */
    int exit_code;
    /**
     * Действие, которое должна совершить оболочка.
     */
    action_t action;
} cmd_result_t;

#define CMD_RESULT(ac, ex) ((cmd_result_t){ .action=ac, .exit_code=ex })
#define CMD_EXIT(ex) CMD_RESULT(CMD_ACT_CONTINUE, ex)
#define CMD_OK CMD_EXIT(0)
#define CMD_QUIT_SHELL(ex) CMD_RESULT(CMD_ACT_EXIT, ex)


cmd_result_t
execute_command(cmd_t* command);

cmd_result_t
execute_command_line(char* command_line);

int
run_interactive_mode();

int
execute_commands_from_file(char* file);


/**
 * Переменная оболочки
 */
typedef struct variable_s {
    char* name;  /**< Variable's name  */
    char* value; /**< Variable's value */
} variable_t;

const char*
get_variable(const char* name);

const variable_t*
get_variables(void);

size_t
get_variables_count(void);

void
set_variable_checked(const char* name, const char* value);

void
set_variable(const char* name, const char* value);

void
unset_variable(const char* name);

#define IN STDIN_FILENO
#define OUT STDOUT_FILENO
#define ERR STDERR_FILENO
enum error_in {
    E_NOMEM,
    E_WRITE,
    E_READ,
    E_MANYARGS,
    E_INVARGS,
    E_FORK,
    E_RED_OP,
    E_DUP,
    E_PIPE,
    E_EXEC,
    E_NOTERM,
    E_UNDEF
};

void
switch_to_noncanonical_mode(void);

void
switch_to_canonical_mode(void);

void
sh_print(const char* message);

void
sh_println(const char* message);

void
sh_print_int(int n);

void
sh_print_uint(unsigned long n);

char*
exit_code_to_str(int n);

void
sh_print_error(enum error_in subj, int do_exit);

char*
read_command_line(void);


#ifndef HAVE_DECL_SETENV

extern int
setenv(const char*, const char*, int);

#endif


#define ALLOC_CHECK_ERR(res)            \
        if (NULL == ((res)))      \
            sh_print_error(E_NOMEM, 1);

extern int sh_argc;

extern char** sh_argv;

extern pid_t sh_pid;

void
say_hello(void);

void
bye(int exit_code);

#include <termios.h>
#include <sys/types.h>


#define ARGS_SIZE 1024

char**
split_command_args(char* line);

// Из массива аргументов (слов, разделителей) делает массив команд
bool_t
parse_command_line(char** args, cmd_t** commands);

void
free_cmd(cmd_t* c);


bool_t
is_space(char c);

bool_t
is_space_or_eol(char c);

#ifndef S_IRWXU
/**
 * Права на чтение, запись и исполнение владельцем.
 */
#define S_IRWXU 0700
#endif

#endif /*_DEFS_H_ */
