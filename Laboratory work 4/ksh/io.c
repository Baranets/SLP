#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#include "defs.h"

#define ERR_MSG(x) "sh: " x

static bool_t term_initialized;
static struct termios savetty;

void
print_prompt();

/**
 * Перейти в неканонический режим.
 *
 * В неканоническом режиме не поддерживается редактирование
 * вводимой строки, ввод не буферизуется и доступен
 * для побайтового считывания без ожидания перевода строки.
 */
void
switch_to_noncanonical_mode()
{
    struct termios tty;
    if (term_initialized) return;
    if (!isatty(IN)) {
        sh_print_error(E_NOTERM, 1);
    }
    tcgetattr(IN, &savetty);
    tty = savetty;
    tty.c_lflag &= ~(ICANON | ECHO | ISIG);
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;
    tcsetattr(IN, TCSAFLUSH, &tty);
    term_initialized = true;
}

/**
 * Перейти в канонический режим.
 *
 * В каноническом режиме поддерживается редактирование
 * вводимой строки, а ввод буферизуется построчно,
 * так что вызов read(2) возвращает по меньшей мере
 * одну строку ввода.
 */
void
switch_to_canonical_mode()
{
    if (!term_initialized) return;
    tcsetattr(IN, TCSAFLUSH, &savetty);
    term_initialized = false;
}


void
sh_print(const char* message)
{
    if (message == NULL) return;
    // assert(NULL != message);
    if (-1 == write(OUT, message, strlen(message))) {
        sh_print_error(E_WRITE, 1);
    }
}

void
sh_print_char(const char c)
{
    if (-1 == write(OUT, &c, 1)) {
        sh_print_error(E_WRITE, 1);
    }
}


void
sh_println(const char* message)
{
    sh_print(message);
    sh_print_char('\n');
}


void
sh_print_int(int n)
{
    char buf[sizeof(int) * 8 + 1];
    size_t i = 0;
    if (0 == n) {
        sh_print("0");
        return;
    }
    if (n < 0) {
        buf[0] = '0';
        ++i;
    }
    for (; n; ++i, n /= 10) {
        buf[i] = '0' + (n % 10);
    }
    buf[i] = '\0';
    sh_print(buf);
}


void
sh_print_uint(unsigned long n)
{
    char buf[sizeof(unsigned long) * 8 + 1];
    size_t i;

    if (0 == n) {
        sh_print("0");
        return;
    }
    for (i = 0; n; ++i, n /= 10) {
        buf[i] = '0' + (n % 10);
    }
    buf[i] = '\0';
    sh_print(buf);
}


char*
exit_code_to_str(int n)
{
    char* buf;
    size_t i, j;

    ALLOC_CHECK_ERR(buf = (char*) malloc(sizeof(char) * 8 + 1));
    if (0 == n) {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }
    for (i = 0; n; ++i, n /= 10) {
        buf[i] = '0' + (n % 10);
    }
    buf[i] = '\0';
    for (j = 0; j < i / 2; ++j) {
        char c = buf[j];
        buf[j] = buf[i - j - 1];
        buf[i - j - 1] = c;
    }
    return buf;
}


void
sh_print_error(enum error_in subj, int do_exit)
{
    switch (subj) {
        case E_WRITE: {
            char err[] = ERR_MSG("stdout write error\n");
            write(ERR, err, sizeof(err));
            break;
        }
        case E_READ: {
            char err[] = ERR_MSG("stdin read error\n");
            write(ERR, err, sizeof(err));
            break;
        }
        case E_RED_OP: {
            char err[] = ERR_MSG("File descriptor not found\n");
            UNUSED(err);
            break;
        }
        case E_MANYARGS: {
            char err[] = ERR_MSG("Too many arguments\n");
            write(ERR, err, sizeof(err));
            break;
        }
        case E_INVARGS: {
            char err[] = ERR_MSG("Invalid arguments \n");
            write(ERR, err, sizeof(err));
            break;
        }
        case E_FORK: {
            char err[] = ERR_MSG("fork error\n");
            write(ERR, err, sizeof(err));
            break;
        }
        case E_DUP: {
            char err[] = ERR_MSG("file descriptors are broken\n");
            write(ERR, err, sizeof(err));
            break;
        }
        case E_PIPE: {
            char err[] = ERR_MSG("Piping error\n");
            write(ERR, err, sizeof(err));
            break;
        }
        case E_EXEC: {
            char* err;
            switch (errno) {
                case ENOENT:
                    err = ERR_MSG("Command not found\n");
                    break;
                case EACCES:
                    err = ERR_MSG("Could not access the executable\n");
                    break;
                default:
                    err = ERR_MSG("Execution error\n");
            }
            write(STDERR_FILENO, err, strlen(err));
            break;
        }
        case E_UNDEF: {
            char err[] = ERR_MSG("Something went wrong\n");
            write(ERR, err, sizeof(err));
            break;
        }
        default: {
            char err[] = ERR_MSG("Check for stray neutrinos\n");
            write(ERR, err, sizeof(err));
        }
    }

    if (do_exit) bye(subj + 1);
}


static void
string_shl(char* str, size_t step)
{
    if (!step) return;
    for (; *str; ++str) {
        *(str - step) = *str;
    }
    *(str - step) = *str;
}


static void
string_shr(char* str, size_t step)
{
    size_t i, len;

    if (!step) return;
    len = strlen(str);
    str[len + 1] = '\0';
    for (i = len; i >= step; --i) {
        str[i] = str[i - step];
    }
}

char*
read_command_line()
{
    switch_to_noncanonical_mode();

    char c[] = {'\0', '\0'};
    char* line;
    size_t pos = 0;
    size_t length = 0;

    ALLOC_CHECK_ERR(line = (char*) calloc(1, 1024));

    print_prompt();

    while (true) {
        if (-1 == read(IN, c, 1)) sh_print_error(E_READ, 1);
        switch (c[0]) {
            case 0x7f:
                /* Backspace */
                if (pos == 0) continue;
                if (pos == length) {
                    sh_print("\b \b");
                    pos--;
                    length--;
                    line[pos] = '\0';
                } else if (pos > 0) {
                    size_t i;
                    sh_print_char('\b');
                    for (i = pos; i < length; ++i) {
                        sh_print_char(line[i]);
                    }
                    sh_print("  ");
                    for (i = pos; i <= length + 1; ++i) {
                        sh_print_char('\b');
                    }
                    string_shl(line + pos, 1);
                    pos--;
                    length--;
                }
                continue;

            case 0x1b: {
                /* Arrow key */
                read(IN, c, 1);
                if (c[0] != 0x5b) continue;
                read(IN, c, 1);

                switch (c[0]) {
                    case 0x41:
                        /* Up */
                        break;
                    case 0x42:
                        /* Down */
                        break;
                    case 0x43:
                        /* Right */
                        if (pos == length) continue;
                        sh_print("\x1b\x5b\x33\x7e");
                        write(OUT, line + pos, 1);
                        pos++;
                        break;
                    case 0x44:
                        /* Left */
                        if (pos == 0) continue;
                        sh_print("\b");
                        pos--;
                        break;
                    default:
                        continue;
                }
                break;
            }

            case 0x03:
                /* ^C */
                sh_print("^C\n");
                line[0] = 0;
                return line;
                break;

            case 0x15:
                /* ^U */
                sh_print("\33[2K\r");
                print_prompt();
                pos = 0;
                length = 0;
                line[0] = 0;
                break;

            case 0x04:
                /* ^D */
                bye(0);
                break;

            case 0x0c:
                /* ^L */
                sh_print("\x1b[1;1H\x1b[2J");
                pos = 0;
                length = 0;
                line[0] = 0;
                return line;
                break;

            case 0x0a:
                /* Return */
                sh_print("\n");
                line[length] = '\0';
                {
                    size_t i;
                    for (i = 0; i < length; ++i) {
                        if (line[i] == '\n' || line[i] == '\r') {
                            line[i] = ' ';
                        }
                    }
                }
                return line;

            default:
                if (pos == length) {
                    sh_print(c);
                    line[pos] = c[0];
                } else {
                    size_t i;

                    string_shr(line + pos, 1);
                    line[pos] = c[0];
                    sh_print(line + pos);
                    for (i = pos; i < length; ++i) {
                        sh_print_char('\b');
                    }

                }
                pos++;
                length++;
        }
    }
}

void
print_prompt()
{
    sh_print("[sh] ");
    sh_print(getenv("PWD"));
    sh_print("$ ");
}
