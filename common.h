#ifndef COMMONH
#define COMMONH

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

struct wordcounter{
    char*  word;
    size_t counter;
};

typedef struct wordcounter* data_t;

#endif