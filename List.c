#include "List.h"

Node* ListCtor (void)
{
    return (Node*) calloc (1, sizeof (Node));
}

Node* NodeInsAft (Node* last, data_t push)
{
    if (last == NULL)
    {
        Node* new = (Node*) calloc (1, sizeof (Node));
        new->data = push;
        return new;
    }
    Node* new = (Node*) calloc (1, sizeof (Node));
    new->next = last->next;
    last->next = new;
    new->data = push;
    return new;
}

int NodeDeleteAft (Node* node)
{
    if (node->next == NULL)
    {
        return ERR;
    }
    Node* tmpnode = node->next;
    node->next = node->next->next;
    free (tmpnode);
    return NO_ERR;
}

int ListDtor (Node* first)
{
    assert (first != NULL);
    while (NodeDeleteAft (first) != ERR) {;}
    free (first);
    return NO_ERR;
}

