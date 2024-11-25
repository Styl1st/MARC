#include "node.h"
#include <stdlib.h>

void setChildren(t_node* node , int amount){
    node->childs = (t_node*)malloc(amount*sizeof(t_node));
    node->child_amount = amount;
}