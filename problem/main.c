#include "problem.h"

int main (int argc, char* argv[])
{
    long bufsz = 0;
    char* buffer = Read (argv[argc - 1], &bufsz);
    assert (buffer != NULL);
    NormalizeWords (buffer);

    Htab* htab = HtabCtor (16, HashFunc, cmp, fprintelem);

    Node* lofstr = HtabFill (htab, buffer, (size_t) bufsz);

    for (char* instr = Input (); instr != NULL; instr = Input ())
    {
        printf ("word \"%s\" occurs %zd time/s in text\n", instr, Count (htab, instr));
        free (instr);
    }
    if (argv[argc - 2] != NULL && strcmp (argv[argc - 2], "--graph-dump=yes") == 0)
    {
        printf ("Flag: --graph-dump=yes\n");
        GraphDump (htab);
    }
    HtabDtor (htab);
    FreeAllStructs (lofstr);
    free (buffer);

    return 0;
}
