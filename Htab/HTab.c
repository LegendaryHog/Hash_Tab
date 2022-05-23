#include "HTab.h"

size_t gdcounter = 0;

Htab* HtabCtor (size_t capacity, size_t (*HashFunc) (data_t obj), int (*cmp) (data_t obj1, data_t obj2),
void (*fprintelem) (FILE* file, data_t obj))
{
    Htab* htab = (Htab*) calloc (1, sizeof (Htab));
    htab->logfile = fopen ("logs/logfile.txt", "w");
    if (capacity == 0)
    {
        fprintf (htab->logfile, "Capacity is null\n");
        return NULL;
    }
    htab->capacity = capacity;
    htab->buck     = (buck_t*) calloc (htab->capacity, sizeof (buck_t));
    htab->list     = (Node*) calloc (1, sizeof (Node));

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
    else if (fprintelem == NULL)
    {
        fprintf (htab->logfile, "Pointer on printelem function is NULL");
        return NULL;
    }
    htab->cmp        = cmp;
    htab->HashFunc   = HashFunc;
    htab->fprintelem = fprintelem; 
    htab->ctorflag   = 1;
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
    ListDtor (htab->list);
    htab->ctorflag = 0;
    for (size_t i = 0; i < htab->capacity; i++)
    {
        if (htab->buck[i] != NULL)
        {
            free (htab->buck[i]);
        }
    }
    free (htab->buck);
    fclose (htab->logfile);
    free (htab);
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

/*int HtabFill (Htab* htab, char* buffer)
{
    size_t ip = 0;

    while (buffer[ip] != '\0')
    {
        ip += SkipSpaces (buffer + ip);
        HtabAdd (htab, htab->ScanBuf (buffer, &ip));
    }
    return NO_ERR;
}*/

int HtabAdd (Htab* htab, data_t obj)
{
    if (htab->size >  htab->capacity / 2)
    {
        HtabResize (htab);
    }
    size_t hash = htab->HashFunc (obj) % htab->capacity;
    if (htab->buck[hash] == NULL)
    {
        htab->buck[hash] = (buck_t) calloc (1, sizeof (struct inlist));
        htab->buck[hash]->fnode = NodeInsAft (htab->list, obj);
    }
    else
    {
        NodeInsAft (htab->buck[hash]->fnode, obj);
    }
    htab->buck[hash]->size++;
    htab->size++;
    return NO_ERR;
}

void PrintHtab (Htab* htab, FILE* file)
{
    fprintf (file, "\tHTAB [color = darkmagenta, style = filled, fillcolor = aqua, penwidth=3.0,  label = \"Htab:\\n %p | <BUCK> buck:\\n %p | capacity:\\n %zd | size:\\n %zd | ", htab, htab->buck, htab->capacity, htab->size);
    fprintf (file, "HashFunc:\\n %p | cmp:\\n %p | logfile:\\n %p | ctorflag:\\n %d\"];\n", htab->HashFunc, htab->cmp, htab->logfile, htab->ctorflag);
}

void PrintBuck (Htab* htab, FILE* file)
{
    fprintf (file, "\tsubgraph BUCKET {\n");
    fprintf (file, "\t\tBUCKET [style = filled, fillcolor = green, penwidth=3.0, color = black, label = \" <bucket> buck:\\n%p", htab->buck);
    for (size_t i = 0; i < htab->capacity; i++)
    {
        if (htab->buck[i] != NULL)
        {
            fprintf (file, " |{ hash:\\n %zd |<buck%zd> node: %p size: %zd}", i, i, htab->buck[i]->fnode, htab->buck[i]->size); 
        }
    }
    fprintf (file, "\"];\n\t}\n");
}

void PrintList (void (*fprintelem) (FILE* file, data_t obj), Node* start, size_t size, size_t index, FILE* file)
{
    if (start == NULL)
    {
        return;
    }
    else
    {
        size_t i = 0;
        size_t k = 0;
        for (Node* node = start; k < size; node = node->next)
        {
            fprintf (file, "\tNODE_%zd_%zd [style = filled, fillcolor = lightblue, penwidth = 2.5, label = \"{<node%zd> node:\\n%p | elem:\\n", index, i, i, node);
            fprintelem (file, node->data);
            fprintf (file, " | <next%zd> next:\\n%p}\"];\n", i, node->next);
            i++;
            k++;
        }
        fprintf (file, "\tBUCKET:buck%zd -> NODE_%zd_0:node0:w[dir = both, arrowtail = dot];\n", index, index);
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
        if (htab->buck[i] != NULL)
            PrintList (htab->fprintelem, htab->buck[i]->fnode, htab->buck[i]->size, i, graph);
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

int HtabResize (Htab* htab)
{
    for (size_t i = 0; i < htab->capacity; i++)
    {
        if (htab->buck[i] != NULL)
        {
            free (htab->buck[i]);
        }
    }
    htab->capacity *= 2;
    htab->buck = (buck_t*) realloc (htab->buck, htab->capacity * sizeof (buck_t));
    for (size_t i = 0; i < sizeof (buck_t) * htab->capacity; i++)
    {
        ((char*)htab->buck)[i] = 0;
    }
    Node* oldlist = htab->list;
    htab->list = (Node*) calloc (1, sizeof (Node));
    while (oldlist->next != NULL)
    {
        HtabAdd (htab, oldlist->next->data);
        NodeDeleteAft (oldlist);
    }
    free (oldlist);
    return NO_ERR;
}

data_t* HtabFind (Htab* htab, data_t obj)
{
    size_t hash = htab->HashFunc (obj) % htab->capacity;
    size_t i = 0;
    if (htab->buck[hash] == NULL)
    {
        return NULL;
    }
    for (Node* node = htab->buck[hash]->fnode; i < htab->buck[hash]->size; node = node->next)
    {
        if (htab->cmp (obj, node->data) == 0)
        {
            return &(node->data);
        }
        i++;
    }
    return NULL;
}