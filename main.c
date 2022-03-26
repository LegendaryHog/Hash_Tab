#include "HTab.h"

size_t HashFunc (data_t obj)
{
    return 0*obj;
}

int cmp (data_t obj1, data_t obj2)
{
    return obj1 - obj2;
}

data_t ScanBuf (char* buffer, size_t* ptrip)
{
    data_t new = 0;
    (*ptrip) += sscanf (buffer + *ptrip, "%d", &new);
    return new;
}

void fprintelem (FILE* file, data_t obj)
{
    fprintf (file, "%d", obj);
}



int main (void)
{
    Htab* htab = HtabCtor (2, HashFunc, cmp, ScanBuf, fprintelem);
    for (int i = 0; i < 10; i++)
    {
        HtabAdd (htab, 10 + i);
    }
    GraphDump (htab);
    HtabDtor (htab);

    return 0;
}