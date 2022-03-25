#ifndef LISTH
#define LISTH

#include "common.h" 

typedef int data_t;

typedef struct Node {
    data_t data;
    struct Node* next;
} Node;

enum ERRORS {
    NO_ERR = 0,
    ERR = 1,
};

Node* List_Ctor       (void);
int   List_Dtor       (Node* node);
Node* Node_Ins_Aft    (Node* last, data_t push);
int   Node_Delete_Aft (Node* elem);
int   List_Dump       (Node* node);

#endif
