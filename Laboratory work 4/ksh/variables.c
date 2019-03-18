#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "defs.h"

#define MAX_VARIABLES 1024

static variable_t vars[MAX_VARIABLES];
static size_t vars_count;

const char*
get_variable(const char* name)
{
    size_t i;

    assert(NULL != name);

    for (i = 0; i < vars_count; ++i) {
        if (!strcmp(vars[i].name, name))
            return vars[i].value;
    }
    return getenv(name);
}


const variable_t*
get_variables()
{
    return vars;
}


size_t
get_variables_count()
{
    return vars_count;
}


void
set_variable_checked(const char* name, const char* value)
{
    if (!strcmp(name, "$")) return;
    if (!strcmp(name, "?")) return;
    set_variable(name, value);
}

void
unset_variable(const char* name)
{
    size_t i;
    bool_t checked = false;
    if (!strcmp(name, "$")) return;
    if (!strcmp(name, "?")) return;
    for (i = 0; i < vars_count; i++) {
        if (!checked) {
            variable_t* v = &vars[i];
            if (!strcmp(v->name, name)) {
                free(v->value);
                free(v->name);
                checked = true;
                if (i + 1 < vars_count) {
                    vars[i] = vars[i + 1];
                }
            }
        } else {
            if (i + 1 < vars_count) {
                vars[i] = vars[i + 1];
            }
        }
    }
    if (checked) {
        vars_count--;
    }
    return;
}


void
set_variable(const char* name, const char* value)
{
    size_t i;
    for (i = 0; i < vars_count; ++i) {
        variable_t* v = &vars[i];
        if (!strcmp(v->name, name)) {
            free(v->value);
            ALLOC_CHECK_ERR(v->value = (char*) malloc(strlen(value) + 1));
            strcpy(v->value, value);
            return;
        }
    }
    ALLOC_CHECK_ERR(vars[vars_count].name = (char*) malloc(strlen(name) + 1));
    ALLOC_CHECK_ERR(vars[vars_count].value = (char*) malloc(strlen(value) + 1));
    strcpy(vars[vars_count].name, name);
    strcpy(vars[vars_count].value, value);
    ++vars_count;
}
