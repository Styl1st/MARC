#ifndef STDLIST_H
#define STDLIDT_H

#include "node.h"

typedef struct s_std_list{
    t_node* node;
    struct s_std_list* next;
}t_std_list;


#endif