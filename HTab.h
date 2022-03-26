#ifndef HTABH
#define HTABH

#include "List.h"
#include "common.h"

#define LEN0 55

enum SEARCH {
    FIND = 0xF15D,
    NOT_FIND = 0x501F15D,
};
typedef struct Htab {
    Node** buck;
    size_t capacity;
    size_t size;
    size_t (*HashFunc)  (data_t obj);
    int    (*cmp)       (data_t obj1, data_t obj2);
    data_t (*ScanBuf)   (char* buffer, size_t* ptrip);
    void   (*fprintelem)(FILE* file, data_t obj);
    FILE*  logfile;
    int    ctorflag;
} Htab;

Htab*  HtabCtor   (size_t capacity, size_t (*HashFunc) (data_t obj), int (*cmp) (data_t obj1, data_t obj2),
data_t (*ScanBuf) (char* buffer, size_t* ptrip), void (*fprintelem) (FILE* file, data_t obj));

int    HtabDtor   (Htab* htab);

int    HtabFill   (Htab* htab, char* buffer);

int    HtabFind   (Htab* htab, data_t obj);

int    HtabResize (Htab* htab);

int    HtabAdd    (Htab* htab, data_t obj);

size_t SkipSpaces (char* text); 

int    GraphDump  (Htab* htab);

#endif