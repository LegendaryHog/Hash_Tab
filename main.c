#include "HTab.h"

size_t HashFunc (data_t obj);

int cmp (data_t obj1, data_t obj2);

size_t ScanBuf (char* buffer, data_t new);

void fprintelem (FILE* file, data_t obj);

Node* HtabFill (Htab* htab, char* buffer, size_t len);

void NormalizeWords (char* buffer);

char* Read (char* filename, long* ptrbufsz);

char my_tolower (char symb);

int strcmp_tolower (char* str1, char* str2);

int FreeAllStructs (Node* first);

size_t power (char base, char pow);

int main (void)
{
    long bufsz = 0;
    char* buffer = Read ("Text.txt", &bufsz);
    NormalizeWords (buffer);

    Htab* htab = HtabCtor (16, HashFunc, cmp, fprintelem);

    Node* lofstr = HtabFill (htab, buffer, (size_t) bufsz);

    GraphDump (htab);
    HtabDtor (htab);
    FreeAllStructs (lofstr);
    free (buffer);

    return 0;
}

char* Read (char* filename, long* ptrbufsz)
{
    FILE* text = fopen (filename, "r");
    fseek (text, 0, SEEK_SET);
    long start = ftell (text);
    fseek (text, 0, SEEK_END);
    long end = ftell (text);
    fseek (text, 0, SEEK_SET);
    *ptrbufsz = end - start;

    char* buffer = (char*) calloc (*ptrbufsz + 1, sizeof (char));
    fread (buffer, sizeof (char), *ptrbufsz, text);
    fclose (text);
    return buffer;
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

Node* HtabFill (Htab* htab, char* buffer, size_t len)
{
    size_t ip = 0;
    Node* first = ListCtor (); 

    while (ip < len)
    {
        data_t new = (data_t) calloc (1, sizeof (char*) + sizeof (size_t));
        NodeInsAft (first, new);
        ip += ScanBuf (buffer + ip, new);
        new->counter = 1;
        data_t* find = HtabFind (htab, new); 
        if (find != NULL)
        {
            (*find)->counter++;
        }
        else
        {
            HtabAdd (htab, new);
        }
        while (buffer[ip] == '\0' && ip < len)
        {
            ip++;
        }
    }
    return first;
}

char my_tolower (char symb)
{
    if (symb >= 'A' && symb <= 'Z')
    {
        return symb + ('a' - 'A');
    }
    else
    {
        return symb;
    }
}

int strcmp_tolower (char* str1, char* str2)
{
    size_t i = 0;

    while (my_tolower (str1[i]) == my_tolower (str2[i]) && str1[i] != '\0' && str2[i] != '\0')
    {
        i++;
    }

    return my_tolower(str1[i]) - my_tolower(str2[i]);
}

size_t power (char base, char pow)
{
    size_t res = 1;
    for (char i = 0; i < pow; i++)
    {
        res *= (size_t) base;
    }
    return res;
}

size_t HashFunc (data_t obj)
{
    size_t len = strlen (obj->word);
    size_t num = 0;
    for (size_t i = 0; i < len; i++)
    {
        char ch = my_tolower (obj->word[i]);
        num += power (ch, 21 * ch);
    }
    return 3 * len * num / 2;
}

int cmp (data_t obj1, data_t obj2)
{
    return strcmp_tolower(obj1->word, obj2->word);
}

size_t ScanBuf (char* buffer, data_t new)
{
    new->word = buffer;
    return strlen (buffer);
}

void fprintelem (FILE* file, data_t obj)
{
    fprintf (file, "word: %s\\n counter: %zd", obj->word, obj->counter);
}

int FreeAllStructs (Node* first)
{
    for (Node* node = first->next; node != NULL; node = node->next)
    {
        free (node->data);
    }
    ListDtor (first);
    return 0;
}