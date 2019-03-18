#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "defs.h"

bool_t
is_space(char c)
{
    return (c == ' ')
           || (c == '\t');
}


bool_t
is_space_or_eol(char c)
{
    return is_space(c) || (c == '\n');
}


static void
skip_spaces(char** line)
{
    while (is_space(**line)) {
        (*line)++;
    }
}


static bool_t
is_separator(char c)
{
    return (c == ';' || c == '\n' || c == '|' || c == '&' || c == '>' || c == '<');
}


// Считать из строки разделитель или следующее слово (аргумент) до пробела или разделителя
static char*
get_arg(const char* line)
{
    size_t p = 0; // длина считанного слова (разделителя или аргумента)
    char* arg;
    if (is_separator(line[0])) {
        p = 1;
        goto extract_arg;
    }

    while (line[p] && !is_space(line[p]) && !is_separator(line[p])) {
        ++p;
    }

    extract_arg:
    ALLOC_CHECK_ERR(arg = (char*) malloc(p + 1));
    strncpy(arg, line, p);
    arg[p] = '\0';
    return arg;
}


static void
put(char** vec, const char* item)
{
    size_t i;
    assert(vec);
    assert(item);
    for (i = 0; NULL != vec[i]; ++i) {}
    if (i + 1 > ARGS_SIZE) {
        sh_print_uint(i);
        sh_print("\n");
        sh_print_error(E_MANYARGS, 1);
    }
    ALLOC_CHECK_ERR(vec[i] = (char*) malloc(strlen(item) + 1));
    strcpy(vec[i], item);
    vec[i][strlen(item)] = '\0';
    vec[i + 1] = NULL;
}

/*find var in var's array and unfold it*/
static void
substitute_variable(char* line)
{
    size_t i;
    char* subs;
    const char* env;

    if (line[0] != '$') return;
    ALLOC_CHECK_ERR(subs = (char*) malloc(strlen(line) + 1));
    for (i = 0; line[i]; ++i) {}
    for (; 0 != 1 + i; --i) {
        strncpy(subs, line + 1, i);
        subs[i + 1] = '\0';
        if (NULL != (env = get_variable(subs))) {
            size_t v_len = strlen(env);
            size_t l_len = strlen(line);
            char* buf;
            ALLOC_CHECK_ERR(buf = (char*) malloc(l_len + v_len + 1));
            strcpy(buf, env);
            strcat(buf, line + i + 1);
            strcpy(line, buf);
            free(buf);
            goto substitute_variable_clean;
        }
    }
    for (i = 0; line[i]; ++i) {}
    for (; 0 != 1 + i; --i) {
        strncpy(subs, line + 1, i);
        subs[i + 1] = '\0';
        if (NULL != (env = getenv(subs))) {
            size_t v_len = strlen(env);
            size_t l_len = strlen(line);
            char* buf;
            ALLOC_CHECK_ERR(buf = (char*) malloc(l_len + v_len + 1));
            strcpy(buf, env);
            strcat(buf, line + i + 1);
            strcpy(line, buf);
            free(buf);
            goto substitute_variable_clean;
        }
    }
    substitute_variable_clean:
    free(subs);
    return;
}

// Разбивает строку на массив слов -- аргументов и разделителей, пробелы пропускаются
char**
split_command_args(char* line)
{
    size_t pos;
    char** args;

    ALLOC_CHECK_ERR(args = (char**) malloc(ARGS_SIZE * sizeof(char*)));
    args[0] = NULL;
    for (pos = 0; line[pos]; ++pos) {
        substitute_variable(line + pos);
    }
    pos = 0;
    while (line[pos]) {
        char* arg;
        skip_spaces(&line);
        if (!line[0]) break;
        arg = get_arg(line);
        line += strlen(arg);
        put(args, arg);
    }
    return args;
}


int
string_to_int(char* x)
{
    int sum = 0;
    while (*x) {
        sum = 10 * sum + *x - '0';
        x++;
    }
    return sum;
}


// Создаёт структуру команды со значениями флагов по умолчанию,
// где массив аргументов (argv) -- это срез массива args
// с first'ного элемента по last'ный.
static cmd_t*
make_cmd(char** args, size_t first, size_t last)
{
    size_t i;
    cmd_t* c;

    assert(last >= first);
    ALLOC_CHECK_ERR(c = (cmd_t*) malloc(sizeof(cmd_t)));
    ALLOC_CHECK_ERR(c->argv = (char**) malloc(sizeof(char*)
                                              * (last - first + 2)));

    c->argc = 0;
    c->in = 0;
    c->out = 1;
    c->is_piped = false;
    c->is_forward = false;
    c->is_thrown = false;
    c->is_fwd_in = false;
    c->is_fwd_to_stream = false;
    for (i = first; i <= last; ++i) {
        c->argv[c->argc++] = args[i];
        args[i] = NULL;
    }
    c->argv[c->argc] = NULL;
    return c;
}

/*push line to commands*/
// Преобразовать массив аргументов (полученный при split_command_args) в массив команд
bool_t
parse_command_line(char** args, cmd_t** commands)
{
    size_t first, length, i;
    bool_t pipe_next = false;

    assert(NULL != commands);
    commands[0] = NULL;

    first = length = 0;
    for (i = 0; args[i]; ++i) {
        // Если мы встретили разделитель ИЛИ дошли до последнего аргумента
        if (is_separator(args[i][0]) || !args[i + 1]) {

            // Предыдущий элемент массива -- конец команды.
            size_t last = i - 1;

            // Однако если здесь не разделитель, то здесь таки последний
            // элемент массива, и он должен быть включен в команду
            if (!is_separator(args[i][0]))
                last = i;

            // Создать структуру, содержащую команду
            // с аргументами из массива с first по last.
            // Дальше эту команду будем называть текущей.
            commands[length] = make_cmd(args, first, last);

            // Если предыдущая команда заканчивалась на |,
            // нужно для текущей команды STDIN перенаправить
            // из STDOUT предыдущей.
            if (pipe_next) {
                commands[length]->in = 2;
            }

            // Если здесь стоит палочка |,
            // то поставить текущей команде флаг перенаправления
            // и указать, чтобы следующая команда брала STDIN
            // из STDOUT'а текущей.
            if (args[i] && !strcmp(args[i], "|")) {
                commands[length]->out = 2;
                commands[length]->is_piped = true;
                pipe_next = true;
            } else {
                pipe_next = false;
            }

            // Если текущая команда должна быть связана
            // со следующей конвейером,
            if (pipe_next) {
                // а следующей команды просто нет ИЛИ следующее слово -- разделитель,
                if (args[i + 1] == NULL || is_separator(args[i + 1][0])) {
                    // то освободить память, выделенную для текущей команды,
                    // потому что строка команд неправильно составлена,
                    free_cmd(commands[length]);
                    // поместить NULL на место последней команды, ибо у нас
                    // массив команд заканчивается нуль-терминатором,
                    commands[length] = NULL;
                    // и вернуть false типа "всё плохо, команды неправильные"
                    return false;
                }
            }

            // Если в этом месте стоит >>,
            if (args[i] && !strcmp(args[i], ">") && !strcmp(args[i + 1], ">")) {
                // а последняя команда перенаправляется в следующую
                // либо дальше NULL, триггернуться
                if (commands[length]->is_piped || args[i + 1] == NULL)
                    return false;

                // Указать, что текущая команда добавляет в конец файла
                commands[length]->is_appending = true;
                // И указать файл
                ALLOC_CHECK_ERR(commands[length]->redirect_filename =
                                        (char*) malloc(strlen(args[i + 2] + 1)));
                strcpy(commands[length]->redirect_filename, args[i + 2]);
                // i -- первый >, i+1 -- второй >, i+2 -- имя файла, пропустить всё это
                i += 3;
            }
            // Если мы на знаке >,
            if (args[i] && !strcmp(args[i], ">")) {
                // а команда перенаправляется куда-то там
                // или этот символ последний, триггернуться
                if (commands[length]->is_piped || !args[i + 1])
                    return false;

                // Если следующий символ тоже >, указать, что
                // перенаправление осуществляется с записью в конец файла
                if (!strcmp(">", args[i + 1])) {
                    ++i;
                    if (!args[i + 1])
                        return false;
                    commands[length]->is_thrown = true;
                }
                // Если дальше идёт амперсанд,
                if (args[i + 1][0] == '&') {
                    // редиректнуть по номеру ФД
                    commands[length]->stream_descriptor = string_to_int(args[i + 2]);
                    commands[length]->is_fwd_to_stream = true;
                    commands[length]->is_forward = true;
                    // пропустить >, амперсанд и номер ФД
                    i += 3;
                } else {
                    // нет амперсаса, значит дальше имя фаела, в который редирект
                    commands[length]->is_forward = true;
                    ALLOC_CHECK_ERR(commands[length]->redirect_filename =
                                            (char*) malloc(strlen(args[i + 1] + 1)));
                    strcpy(commands[length]->redirect_filename, args[i + 1]);
                    // пропускаем > и имя файла
                    i += 2;
                }
            }
            // Ага, дарова, тут ещё и перенаправление из файла
            if (args[i] && !strcmp(args[i], "<")) {
                // Если дальше ничего нет, прекратить обработку
                if (commands[length]->is_piped || !args[i + 1])
                    return false;

                // поставить флаг, скопировать имя файла
                commands[length]->is_fwd_in = true;
                ALLOC_CHECK_ERR(commands[length]->name =
                                        (char*) malloc(strlen(args[i + 1] + 1)));
                strcpy(commands[length]->name, args[i + 1]);
                // пропустить < и имя файла
                i += 2;
            }
            // Перейти к следующей команде, вместо неё пока что поставить NULL
            commands[++length] = NULL;
            // Сейчас i равно индексу элемента после всех аргументов текущей команды,
            // а также относящихся к ней разделителей.
            while (args[i]) {
                if (is_separator(args[i][0])) {
                    ++i;
                } else {
                    first = i;
                    last = i;
                    break;
                }
            }
            --i;
        }
    }
    return true;
}

// освободить память, выделенную под команду
void
free_cmd(cmd_t* c)
{
    size_t i;
    for (i = 0; c->argv[i]; ++i) {
        free(c->argv[i]);
    }
    free(c);
}
