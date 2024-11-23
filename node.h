#ifndef UNTITLED1_NODE_H
#define UNTITLED1_NODE_H

#include "moves.h"

typedef struct s_node{
    int weight;
    t_move move;
    struct s_node* childs;
} t_node;

void setChildren(t_node* , int);

#endif