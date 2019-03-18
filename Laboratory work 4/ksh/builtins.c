#include "builtins.h"
#include "stdlib.h"

cmd_result_t
builtin_cd(size_t argc, char** argv)
{
    if (shell_settings.verbose) sh_print("cd builtin\n");
    if (argc == 1) {
        char* home = getenv("HOME");
        chdir(home);
        setenv("PWD", home, 1);
        return CMD_OK;
    }

    if (chdir(argv[1])) {
        sh_print_error(E_UNDEF, 0);
        return CMD_EXIT(2);
    }

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    setenv("PWD", cwd, 1);

    return CMD_OK;
}

cmd_result_t
builtin_exit(size_t argc, char** argv)
{
    if (shell_settings.verbose) sh_print("exit builtin");
    cmd_result_t result;
    if (argc >= 2) {
        int exit_code = atoi(argv[1]);
        result = CMD_QUIT_SHELL(exit_code);
    } else {
        result = CMD_QUIT_SHELL(0);
    }
    sh_print("¡Adios!\n");
    return result;
}

cmd_result_t
builtin_set(size_t argc, char** argv)
{
    if (shell_settings.verbose) sh_print("set builtin\n");
    const variable_t* vars = get_variables();
    size_t vars_count = get_variables_count();
    size_t i;

    if (argc == 1) {
        for (i = 0; i < vars_count; ++i) {
            sh_print(vars[i].name);
            sh_print("=");
            sh_println(vars[i].value);
        }
        return CMD_OK;
    }
    set_variable_checked(argv[1], argc == 3 ? argv[2] : "");
    return CMD_OK;
}

cmd_result_t
builtin_unset(size_t argc, char** argv)
{
    if (shell_settings.verbose) sh_print("unset builtin\n");
    if (argc == 1) {
        sh_print_error(E_INVARGS, 1);
    } else if (argc != 2) {
        sh_print_error(E_MANYARGS, 1);
    } else {
        unset_variable(argv[1]);
    }
    return CMD_EXIT(0);
}
