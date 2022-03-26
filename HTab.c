#include "HTab.h"

size_t gdcounter = 0;

Htab* HtabCtor (size_t capacity, size_t (*HashFunc) (data_t obj), int (*cmp) (data_t obj1, data_t obj2),
data_t (*ScanBuf) (char* buffer, size_t* ptrip), void (*fprintelem) (FILE* file, data_t obj))
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
    else if (ScanBuf == NULL)
    {
        fprintf (htab->logfile, "Pointer on filling function is NULL\n");
        return NULL;
    }
    else if (fprintelem == NULL)
    {
        fprintf (htab->logfile, "Pointer on printelem function is NULL");
        return NULL;
    }
    htab->cmp       = cmp;
    htab->HashFunc  = HashFunc;
    htab->ScanBuf    = ScanBuf;
    htab->fprintelem = fprintelem; 
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

    while (buffer[ip] != '\0')
    {
        ip += SkipSpaces (buffer + ip);
        HtabAdd (htab, htab->ScanBuf (buffer, &ip));
    }
    return NO_ERR;
}

int HtabAdd (Htab* htab, data_t obj) //Not work
{
    size_t hash = htab->HashFunc (obj) % htab->capacity;
    if (htab->buck[hash] == NULL)
    {
        htab->buck[hash] = NodeInsAft (htab->buck[hash], obj);
    }
    else
    {
        NodeInsAft (htab->buck[hash], obj);
    }
    htab->size++;
    return NO_ERR;
}

void PrintHtab (Htab* htab, FILE* file)
{
    fprintf (file, "\tHTAB [color = darkmagenta, style = filled, fillcolor = aqua, penwidth=3.0,  label = \"Htab:\\n %p | <BUCK> buck:\\n %p | capacity:\\n %zd | size:\\n %zd | ", htab, htab->buck, htab->capacity, htab->size);
    fprintf (file, "HashFunc:\\n %p | cmp:\\n %p | ScanBuf:\\n %p | logfile:\\n %p | ctorflag:\\n %d\"];\n", htab->HashFunc, htab->cmp, htab->ScanBuf, htab->logfile, htab->ctorflag);
}

void PrintBuck (Htab* htab, FILE* file)
{
    fprintf (file, "\tsubgraph BUCKET {\n");
    fprintf (file, "\t\tBUCKET [style = filled, fillcolor = green, penwidth=3.0, color = black, label = \" <bucket> buck:\\n%p", htab->buck);
    for (size_t i = 0; i < htab->capacity; i++)
    {
        fprintf (file, " | {hash:\\n %zd |<buck%zd> node:\\n%p}", i, i, htab->buck[i]);
    }
    fprintf (file, "\"];\n\t}\n");
}

void PrintList (void (*fprintelem) (FILE* file, data_t obj), Node* start, size_t index, FILE* file)
{
    if (start == NULL)
    {
        return;
    }
    else
    {
        size_t i = 0;
        for (Node* node = start; node != NULL; node = node->next)
        {
            fprintf (file, "\tNODE_%zd_%zd [style = filled, fillcolor = lightblue, penwidth = 2.5, label = \"<node%zd> node:\\n%p | elem:\\n", index, i, i, node);
            fprintelem (file, node->data);
            fprintf (file, " | <next%zd> next:\\n%p\"];\n", i, node->next);
            i++;
        }
        fprintf (file, "\tBUCKET:buck%zd -> NODE_%zd_0:node0[dir = both, arrowtail = dot];\n", index, index);
        fprintf (file, "\tBUCKET:buck%zd -> NODE_%zd_0[color = invis];\n", index, index);
        for (size_t j = 0; j < i - 1; j++)
        {
            fprintf (file, "\tNODE_%zd_%zd -> NODE_%zd_%zd [color = invis];\n", index, j, index, j+1);
            fprintf (file, "\tNODE_%zd_%zd:next%zd -> NODE_%zd_%zd:next%zd[color = invis];\n", index, j, j, index, j+1, j+1);
            fprintf (file, "\tNODE_%zd_%zd:node%zd -> NODE_%zd_%zd:node%zd[color = invis];\n", index, j, j, index, j+1, j+1);
            fprintf (file, "\tNODE_%zd_%zd:next%zd -> NODE_%zd_%zd:node%zd[dir = both, arrowtail = dot, color = red];\n", index, j, j, index, j+1, j+1);
        }
    }
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
    fprintf (graph, "\tHTAB:BUCK -> BUCKET:bucket[dir = both, arrowtail = dot, color = darkmagenta];\n");
    for (size_t i = 0; i < htab->capacity; i++)
    {
        PrintList (htab->fprintelem, htab->buck[i], i, graph);
    }
    fprintf (graph, "}\n");
    fclose (graph);
    char* cmd_mes = (char*) calloc (LEN0 + gdcounter, sizeof (char));
    sprintf (cmd_mes, "dot -Tpng logs/graph_dump.dot -o logs/Graph_Dump%zd.png", gdcounter);
    system (cmd_mes);
    free (cmd_mes);
    //system ("rm logs/graph_dump.dot");
    gdcounter++;
    return NO_ERR;
}