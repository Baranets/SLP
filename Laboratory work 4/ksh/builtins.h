#ifndef LAB4_BUILTINS_H
#define LAB4_BUILTINS_H

#include "defs.h"
#include "stdint.h"

cmd_result_t
builtin_cd(size_t argc, char** argv);

cmd_result_t
builtin_exit(size_t argc, char** argv);

cmd_result_t
builtin_set(size_t argc, char** argv);

cmd_result_t
builtin_unset(size_t argc, char** argv);

cmd_result_t
builtin_setenv(size_t argc, char** argv);

#include "defs.h"

#endif //LAB4_BUILTINS_H
