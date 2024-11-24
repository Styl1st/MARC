#ifndef SELECTION_TREE_H
#define SELECTION_TREE_H

typedef struct s_node {
    struct s_node *left;
    struct s_node *right;
    int value;
} t_node;

typedef struct s_tree {
    t_node *root;
} t_tree;

t_tree createTree();
t_node* createNode(int value);
void insert(t_tree *tree, int value);
t_node* findMinLeaf(t_tree tree);
int getPathToLeaf(t_node *root, t_node *target, t_node **path, int *pathIndex);
void printPath(t_node **path, int pathIndex);

#endif // SELECTION_TREE_H