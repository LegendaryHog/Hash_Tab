#ifndef LISTH
#define LISTH

#include "common.h" 

enum ERRORS {
    NO_ERR = 0,
    ERR = 1,
};
typedef struct Node {
    data_t data;
    struct Node* next;
} Node;

Node* ListCtor      (void);
int   ListDtor      (Node* node);
Node* NodeInsAft    (Node* last, data_t push);
int   NodeDeleteAft (Node* elem);
int   ListDump      (Node* node);

#endif
