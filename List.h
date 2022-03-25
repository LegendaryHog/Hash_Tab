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

Node* List_Ctor       (void);
int   List_Dtor       (Node* node);
Node* Node_Ins_Aft    (Node* last, data_t push);
int   Node_Delete_Aft (Node* elem);
int   List_Dump       (Node* node);

#endif
