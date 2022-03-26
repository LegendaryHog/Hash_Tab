#include "HTab.h"

Htab* HtabCtor (size_t capacity, size_t (*HashFunc) (data_t obj), int (*cmp) (data_t obj1, data_t obj2), int (*AddBuf) (char* buffer, size_t* ptrip))
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
    htab->AddBuf  = AddBuf; 
    htab->ctorflag = 1;
    return htab;
}

int HtabDtor (Htab* htab)
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
            ListDtor (htab->buck[i]);
        }
    }
    htab->ctorflag = 0;
    free (htab->buck);
    fclose (htab->logfile);
    return NO_ERR;
}

size_t SkipSpaces (char* text)
{
    size_t ip = 0;
    while (text[ip] == ' ' || text[ip] == '\n' || text[ip] == '\r' || text[ip] == '\t')
    {
        ip++;
    }
    return ip;
}

int HtabFill (Htab* htab, char* buffer)
{
    size_t ip = 0;

    while (buffer[ip] != NULL)
    {
        ip += SkipSpaces (buffer + ip);
        data_t new = htab->AddBuf (buffer, &ip);
        size_t hashnew = htab->HashFunc (new) % htab->capacity;
        htab->buck[hashnew] = NodeInsAft (htab->buck[hashnew], new);
        htab->size++;
    }
    return NO_ERR;
}

int HtabAdd (Htab* htab, data_t obj)
{
    szie_t hash = htab->HashFunc (obj) % hash->capacity;
    htab->buck[hash] = NodeInsAft (htab->buck[hash], obj);
    retrun NO_ERR;
}

void PrintHTab (Htab* htab, FILE* file)
{
    fprintf (file, "\tHTAB [label = \"Htab:\n %p | <BUCK> buck:\n %p | capacity:\n %zd | size:\n %zd | ", htab, htab->buck, htab->capacity, htab->size);
    fprintf (file, "HashFunc:\n %p | cmp\n %p | AddBuf:\n %p | logfile:\n %p | ctorflag:\n %d\"]\n", htab->HashFunc, htab->cmp, htab->AddBuf, htab->logfile, htab->ctorflag);
}

void PrintBuck (Htab* htab, FILE* file)
{
    fprintf (file, "\tsubgraph {\n");
    fprintf (file, "\t\trankdir = TB\n\t\tstyle = filled\n\t\tfillcolor = black\n\t\tlabel = \"<BUCK>buck:\n%p\"\n", htab->buck);
    while (size_t i = 0; i < htab->capacity; i++)
    {
        fprintf (file, "\t\t BUCK_%zd[color = black, style = filled, fillcolor = white, label = \"{hash: %zd |<buck%zd> node:\n%p}\"]\n", i, i, i, htab->buck[i]);
    }
    fprintf (file, "\t}\n");
}

void PrintList (Node* start, size_t index, FILE* file)
{
    fprintf (file, "\t");
}

int GraphDump (Htab* htab)
{
    assert (htab != NULL);

    FILE* graph = fopen ("logs/graph_dump.dot", "w");
    fprintf (graph, "digraph G{\n");
    fprintf (graph, "\trankdir=LR;\n");
    fprintf (graph, "\tnode[color=\"red\",shape=record];\n");
    PrintHtab (htab, graph);
    PrintBuck (htab, graph);
    fprintf (graph, "\tHTAB:BUCK -> BUCK");
    for (size_t i = 0; i < htab->capacity; i++)
    {
        PrintList (htab->buck[i], i, graph);
    }
}