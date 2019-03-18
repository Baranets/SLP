#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include "defs.h"
#include "builtins.h"

// Массив содержит ФД, соответствующие двум
// концам неименованного канала:
//   pipefd[0] -- сторона для чтения;
//   pipefd[1] -- сторона для записи.
static int pipefd[2];

cmd_result_t
execute_command(cmd_t* command)
{
    pid_t pid;

    char** args = command->argv;
    if (args == NULL) return CMD_EXIT(1);
    if (command->argc == 0) return CMD_OK;
    if (!strcmp(command->argv[0], "cd")) {
        return builtin_cd(command->argc, command->argv);
    }
    if (!strcmp(command->argv[0], "exit")) {
        return builtin_exit(command->argc, command->argv);
    }
    if (!strcmp(command->argv[0], "set")) {
        return builtin_set(command->argc, command->argv);
    }
    if (!strcmp(command->argv[0], "unset")) {

        return builtin_unset(command->argc, command->argv);
    }
    pid = fork();
    if (pid < 0) {
        sh_print_error(E_FORK, 0);
        return CMD_EXIT(13);
    } else if (0 == pid) {
        // Мы в форке.

        if (command->in != STDIN_FILENO) {
            if (-1 == dup2(command->in, STDIN_FILENO)) {
                sh_print_error(E_DUP, false);
                return CMD_EXIT(15);
            }
        }
        if (command->out != STDOUT_FILENO) {
            if (-1 == dup2(command->out, STDOUT_FILENO)) {
                sh_print_error(E_DUP, true);
                return CMD_EXIT(15);
            }
        }
        if (command->in != STDIN_FILENO) close(pipefd[0]);
        if (command->out != STDOUT_FILENO) close(pipefd[1]);
        if (command->is_appending) {
            if (shell_settings.verbose) {
                printf("Appending to file: %s\n", command->redirect_filename);
            }
            int fd = open(command->redirect_filename,
                          O_RDWR | O_CREAT | O_APPEND, S_IRWXU);

            if (fd == -1) {
                sh_print_error(E_RED_OP, 0);
                return CMD_EXIT(16);
            }
            if (dup2(fd, STDOUT_FILENO) == -1) {
                sh_print_error(E_DUP, 0);
                return CMD_EXIT(15);
            }
        }

        if (command->is_forward) {
            int fd;
            if (command->is_fwd_to_stream) {
                fd = command->stream_descriptor;
            } else if (command->is_thrown) {
                fd = open(command->redirect_filename,
                          O_RDWR | O_CREAT | O_APPEND, S_IRWXU);
            } else {
                fd = open(command->redirect_filename,
                          O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
            }
            if (-1 == fd) {
                sh_print_error(E_RED_OP, 0);
                return CMD_EXIT(16);
            }
            if (-1 == dup2(fd, STDOUT_FILENO)) {
                sh_print_error(E_DUP, 0);
                return CMD_EXIT(15);
            }
        }
        if (command->is_fwd_in) {
            int fd = open(command->name, O_RDONLY, S_IRWXU);
            if (-1 == fd) {
                sh_print_error(E_RED_OP, 0);
                return CMD_EXIT(16);
            }
            if (-1 == dup2(fd, STDIN_FILENO)) {
                sh_print_error(E_DUP, 0);
                return CMD_EXIT(15);
            }
            close(fd);
        }

        if (-1 == execvp(args[0], args)) {
            sh_print_error(E_EXEC, 0);
            _exit(14);
        }
    } else {
        // Это не форк, это всё ещё мы
        while (true) {
            // Ожидаем, пока наш форк умрёт или остановится
            int status;
            waitpid(pid, &status, WUNTRACED);
            if (WIFEXITED(status)) {
                // Если форк нормально завершил работу, вернём его код возврата
                if (command->is_piped)
                    close(pipefd[1]);
                return CMD_EXIT(WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                // Его отстретили сигналом или он остановился
                return CMD_OK;
            }
        }
    }
    return CMD_OK;
}


int sh_argc;
char** sh_argv;
pid_t sh_pid;

cmd_t** commands;

int
main(int argc, char* argv[])
{
    sh_argc = argc;
    sh_argv = argv;
    sh_pid = getpid();

    shell_settings = DEFAULT_SHELL_SETTINGS;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-c")) {
            if (++i < argc) {
                shell_settings.command_to_execute = argv[i];
            } else {
                char* err_message = "sh: No argument provided for `-c' option\n";
                write(STDERR_FILENO, err_message, strlen(err_message));
                return EINVAL;
            }
        } else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")) {
            shell_settings.verbose = true;
        } else {
            shell_settings.path_to_source_file = argv[i];
        }
    }

    ALLOC_CHECK_ERR(commands = malloc(sizeof(cmd_t) * 1024));
    if (shell_settings.command_to_execute) {
        return execute_command_line(shell_settings.command_to_execute).exit_code;
    } else if (shell_settings.path_to_source_file) {
        return execute_commands_from_file(shell_settings.path_to_source_file);
    } else {
        say_hello();
        return run_interactive_mode();
    }
}

int
execute_commands_from_file(char* file)
{
    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        perror("sh: Could not open file for reading");
        return errno;
    }

    char program[4096];
    ssize_t program_length = read(fd, &program, sizeof(program));

    close(fd);
    switch (program_length) {
        case 0:
            return 0;
        case -1:
            perror("sh: Could not read file");
            return errno;
        default:
            program[program_length] = '\0';
            return execute_command_line(program).exit_code;
    }
}

int
run_interactive_mode()
{
    char* exit_code_str = NULL;
    while (true) {
        char* line = read_command_line();
        switch_to_canonical_mode();

        cmd_result_t result = execute_command_line(line);
        if (result.action == CMD_ACT_EXIT) {
            return result.exit_code;
        } else {
            exit_code_str = exit_code_to_str(result.exit_code);
            set_variable("?", exit_code_str);
        }
    }
}

cmd_result_t
execute_command_line(char* command_line)
{
    commands[0] = NULL;
    char** args = split_command_args(command_line);
    parse_command_line(args, commands); /* organize structures*/
    cmd_result_t result = CMD_OK;
    for (size_t i = 0; commands[i]; ++i) {
        cmd_t* command = commands[i];
        if (command->is_piped) {
            //создаем неименованный канал
            if (-1 == pipe(pipefd)) {
                sh_print_error(E_PIPE, 1);
                return CMD_EXIT(16);
            }
            // и пусть команда выводит в сторону для записи,
            command->out = pipefd[1];
            // а следующая читает из стороны для чтения :)
            cmd_t* next_command = commands[i + 1];
            if (next_command != NULL)
                next_command->in = pipefd[0];
        }
        result = execute_command(command);
        if (result.action == CMD_ACT_EXIT) {
            return result;
        }
    }
    const char* err = exit_code_to_str(result.exit_code);/*set return code*/
    set_variable("?", err);
    return result;
}

void
say_hello()
{
    printf("Welcome!\n");
    switch_to_noncanonical_mode();
    setenv("SHELL", "sh-lab", 1);
    set_variable("?", "0");
}

void
bye(int exit_code)
{
    printf("Bye-bye!\n");
    switch_to_canonical_mode();
    free(commands);
    _exit(exit_code);
}
