#ifndef PROBLEMH
#define PROBLEMH

#include "HTab.h"

size_t  Count         (Htab* htab, char* word);

size_t  HashFunc      (data_t obj);

int     cmp           (data_t obj1, data_t obj2);

size_t  ScanBuf       (char* buffer, data_t new);

void   fprintelem     (FILE* file, data_t obj);

Node*  HtabFill       (Htab* htab, char* buffer, size_t len);

void   NormalizeWords (char* buffer);

char*  Read           (char* filename, long* ptrbufsz);

char   my_tolower     (char symb);

int    strcmp_tolower (char* str1, char* str2);

int    FreeAllStructs (Node* first);

size_t power          (char base, char pow);

#endif
