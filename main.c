#include <stdio.h>
#include "map.h"
#include "selection_tree.h"

int main() {
    t_map map;
    t_tree tree = createTree();
    insert(&tree, 5);
    insert(&tree, 3);
    insert(&tree, 8);
    insert(&tree, 1);
    insert(&tree, 4);

    t_node *path[100]; // Assurez-vous que le tableau est assez grand
    int pathIndex = 0;

    t_node *target = tree.root->left->left; // Par exemple

    if (target != NULL) {
        if (getPathToLeaf(tree.root, target, path, &pathIndex)) {
            printf("Path to the target:\n");
            printPath(path, pathIndex);
        } else {
            printf("Target not found in the tree.\n");
        }
    } else {
        printf("Target n'est pas un pointeur valide.\n");
    }

#if defined(_WIN32) || defined(_WIN64)
    map = createMapFromFile("..\\maps\\example1.map");
#else
    map = createMapFromFile("../maps/example1.map");
#endif

    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    displayMap(map);

    t_node *minLeaf = findMinLeaf(tree);
    if (minLeaf) {
        printf("Minimum leaf value: %d\n", minLeaf->value);
    }

    getPathToLeaf(tree.root, minLeaf, path, &pathIndex);
    printf("Path to minimum leaf: ");
    printPath(path, pathIndex);

    return 0;
}