#include "selection_tree.h"
#include <stdio.h>
#include <stdlib.h>

t_node* createNode(int value) {
    t_node *newNode = (t_node *)malloc(sizeof(t_node));
    if (!newNode) {
        fprintf(stderr, "Memory error\n");
        return NULL;
    }
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

t_tree createTree() {
    t_tree tree;
    tree.root = NULL;
    return tree;
}

void insert(t_tree *tree, int value) {
    t_node *node = (t_node *)malloc(sizeof(t_node));
    if (!node) {
        fprintf(stderr, "Memory allocation error\n");
        return;
    }
    node->value = value;
    node->left = NULL;
    node->right = NULL;

    if (tree->root == NULL) {
        tree->root = node;
    } else {
        t_node *current = tree->root;
        t_node *parent;
        while (1) {
            parent = current;
            if (value < current->value) {
                current = current->left;
                if (current == NULL) {
                    parent->left = node;
                    return;
                }
            } else {
                current = current->right;
                if (current == NULL) {
                    parent->right = node;
                    return;
                }
            }
        }
    }
}

t_node* findMinLeaf(t_tree tree) {
    t_node *current = tree.root;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

int getPathToLeaf(t_node *root, t_node *target, t_node **path, int *pathIndex) {
    if (!root) {
        return 0; // Le nœud racine est NULL, donc le chemin n'existe pas
    }

    path[(*pathIndex)++] = root;

    if (root == target) {
        return 1;
    }

    if ((root->left && getPathToLeaf(root->left, target, path, pathIndex)) ||
        (root->right && getPathToLeaf(root->right, target, path, pathIndex))) {
        return 1;
    }

    (*pathIndex)--;

    return 0;
}

void printPath(t_node **path, int pathIndex) {
    for (int i = 0; i < pathIndex; i++) {
        printf("%d ", path[i]->value);
    }
    printf("\n");
}