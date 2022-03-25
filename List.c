#include "List.h"

Node* List_Ctor (void)
{
    return (Node*) calloc (1, sizeof (Node));
}

Node* Node_Ins_Aft (Node* last, data_t push)
{
    Node* new = (Node*) calloc (1, sizeof (Node));
    new->next = last->next;
    last->next = new;
    new->data = push;
    return new;
}

int Node_Delete_Aft (Node* node)
{
    if (node->next == NULL)
    {
        return ERR;
    }
    Node* tmpnode = node->next;
    node->next = node->next->next;
    return NO_ERR;
}

int List_Dtor (Node* first)
{
    while (Node_Delete_Aft (first) != ERR) {;}
    free (first);
    return NO_ERR;
}