#ifndef HTABH
#define HTABH

#include "List.h"
#include "common.h"
typedef struct Htab {
    Node** buck;
    size_t capacity;
    size_t (*HashFunc) (data_t obj);
    int    (*cmp)      (data_t obj1, data_t obj2);
    data_t (*Add_Buf)  (char* buffer);
    FILE*  logfile;
    int    ctorflag;
} Htab;

Htab*  Htab_Ctor   (size_t capacity, size_t (*HashFunc) (data_t obj), int (*cmp) (data_t obj1, data_t obj2), data_t (*Add_Buf) (char* buffer));

int    Htab_Dtor   (Htab* htab);

data_t Htab_Find   (Htab* htab, data_t obj);

int    Htab_Reszie (Htab* htab);

int    Htab_Add    (Htab* htab, data_t obj);    

#endif