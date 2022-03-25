#include "HTab.h"

Htab* Htab_Ctor (size_t capacity, size_t (*HashFunc) (data_t obj), int (*cmp) (data_t obj1, data_t obj2), int (*Add_Buf) (char* buffer))
{
    Htab* htab = (Htab*) calloc (1, sizeof (Htab));
    htab->logfile = fopen ("logs/logfile.txt", "w");
    if (capacity == 0)
    {
        fprintf (htab->logfile, "Capacity is null\n");
        return NULL;
    }
    htab->capacity = capacity;
    htab->buck     = (Node**) calloc (htab->capacity, sizeof (Node*));
    if (htab->buck == NULL)
    {
        fprintf (htab->logfile, "Allocation error, your capacity is too fucking big UwU: capacity = %zd\n", capacity);
        return NULL;
    }
    else if (cmp == NULL)
    {
        fprintf (htab->logfile, "Pointer on comparator function is NULL\n");
        return NULL;
    }
    else if (HashFunc == NULL)
    {
        fprintf (htab->logfile, "Pointer on hash count function is NULL\n");
        return NULL;
    }
    else if (Fill == NULL)
    {
        fprintf (htab->logfile, "Pointer on filling function is NULL\n");
        return NULL;
    }
    htab->cmp      = cmp;
    htab->HashFunc = HashFunc;
    htab->Add_Buf  = Add_Buf; 
    htab->ctorflag = 1;
    return htab;
}

int Htab_Dtor (Htab* htab)
{
    assert (htab != NULL);
    if (htab->ctorflag == 0)
    {
        printf ("Hash Table already Dtored");
        return ERR;
    }
    for (size_t i = 0; i < htab->capacity; i++)
    {
        if (htab->buck[i] != NULL)
        {
            List_Dtor (htab->buck[i]);
        }
    }
    htab->ctorflag = 0;
    free (htab->buck);
    fclose (htab->logfile);
    return NO_ERR;
}