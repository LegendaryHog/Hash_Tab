#include "HTab.h"

size_t HashFunc (data_t obj);

int cmp (data_t obj1, data_t obj2);

size_t ScanBuf (char* buffer, data_t* ptrip);

void fprintelem (FILE* file, data_t obj);

int HtabFill (Htab* htab, char* buffer, size_t len);

void NormalizeWords (char* buffer);


int main (void)
{
    FILE* text = fopen ("Text.txt", "r");
    fseek (text, 0, SEEK_SET);
    long start = ftell (text);
    fseek (text, 0, SEEK_END);
    long end = ftell (text);
    fseek (text, 0, SEEK_SET);

    char* buffer = (char*) calloc (end - start + 1, sizeof (char));
    fread (buffer, sizeof (char), end - start, text);
    fclose (text);

    NormalizeWords (buffer);

    Htab* htab = HtabCtor (2, HashFunc, cmp, fprintelem);

    HtabFill (htab, buffer, (size_t) (end - start));

    GraphDump (htab);
    HtabDtor (htab);

    return 0;
}

void NormalizeWords (char* buffer)
{
    for (size_t i = 0; buffer[i] != '\0'; i++)
    {
        if (isalpha (buffer[i]) == 0)
        {
            buffer[i] = '\0';
        }
    }
}

int HtabFill (Htab* htab, char* buffer, size_t len)
{
    size_t ip = 0;

    while (ip < len)
    {
        data_t* ptrnew = (data_t*) calloc (1, sizeof (data_t));
        ip += ScanBuf (buffer + ip, ptrnew);
        data_t* find = HtabFind (htab, *ptrnew); 
        if (find != NULL)
        {
            find->counter++;
        }
        else
        {
            HtabAdd (htab, *ptrnew);
        }
        while (buffer[ip] == '\0' && ip < len)
        {
            ip++;
        }
    }
    return NO_ERR;
}
size_t HashFunc (data_t obj)
{
    size_t len = strlen (obj.word);
    size_t num = 0;
    for (size_t i = 0; i < len; i++)
    {
        num += obj.word[i] * obj.word[i];
    }
    return len * num;
}

int cmp (data_t obj1, data_t obj2)
{
    return strcmp (obj1.word, obj2.word);
}

size_t ScanBuf (char* buffer, data_t* ptrnew)
{
    ptrnew->word = buffer;
    return strlen (buffer);
}

void fprintelem (FILE* file, data_t obj)
{
    fprintf (file, "word: %s\\n counter: %zd", obj.word, obj.counter);
}