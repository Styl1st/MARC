#ifndef UNTITLED1_NODE_H
#define UNTITLED1_NODE_H

#include "moves.h"

typedef struct s_node{
    int weight;
    t_move* move;
    int movelist_length;
    int child_amount;
    struct s_node* childs;
} t_node;

void setChildren(t_node* , int);

#endif