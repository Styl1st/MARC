#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "map.h"
#include "tree.h"
#include "loc.h"
#include "node.h"
#include "queue.h"

t_tree buildTree( int , int , t_localisation , t_map);
void buildBranch( t_node* , t_move* , int , int , t_localisation , t_map);
t_move* getRandomMoves(int);
t_move* removeMoveFromArray(t_move* , t_move , int);
void displayMoveset(t_move* , int);
t_tree sampleTree();
t_move* getLowestWeightMove(t_tree*);
void findLowestWeightMove(t_node*, int* , t_move**);


int main() {
    t_map map;
    t_localisation rover;
    // The following preprocessor directive checks if the code is being compiled on a Windows system.
    // If either _WIN32 or _WIN64 is defined, it means we are on a Windows platform.
    // On Windows, file paths use backslashes (\), hence we use the appropriate file path for Windows.
#if defined(_WIN32) || defined(_WIN64)
    map = createMapFromFile(".\\maps\\example1.map");
#else
    map = createMapFromFile("../maps/example1.map");
#endif

    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }
    // printf the costs, aligned left 5 digits
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%-5d ", map.costs[i][j]);
            if(map.costs[i][j]==0){
                rover = loc_init(i , j , NORTH);
                
            }
        }
        printf("\n");
    }
    buildTree(5 , 3 , rover , map);
    t_tree sample_tree = sampleTree();
    t_move* path_to_lowest = getLowestWeightMove(&sample_tree);
    displayMap(map);
    return 0;
}

t_tree buildTree(int moveset_length , int moves_amount , t_localisation localisation , t_map map){
    t_tree tree;
    t_node root;
    tree.root = root;
    tree.root.child_amount = moveset_length;
    tree.root.movelist_length = 0;
    tree.root.weight = map.soils[localisation.pos.x][localisation.pos.y];
    t_move* moveset = getRandomMoves(moveset_length);
    buildBranch(&(tree.root) , moveset , moveset_length , moves_amount , localisation , map);
    return tree;
}

void buildBranch(t_node* node , t_move* moveset , int moveset_length , int moves_amount , t_localisation localisation , t_map map){
    
    setChildren(node , moveset_length);
    for(int i=0 ; i<moveset_length ; i++){
        
        printf("temp node initialised\n");
        t_node tmpnode;

        //préparation de la liste de t_move
        t_move* node_move;
        node_move = (t_move*)malloc(sizeof(t_move)*node->movelist_length+1);
        for(int j=0 ; j<node->movelist_length ; j++){
            node_move[j]=node->move[j];
        }
        node_move[node->movelist_length+1]=moveset[i];
        tmpnode.move = node_move;
        tmpnode.movelist_length=node->movelist_length+1;
        printf("move array prepared\n");
        //fin de preparation

        t_localisation next_localisation = move(localisation , *(node_move));
        if(0<=next_localisation.pos.x && next_localisation.pos.x<=map.x_max && 0<=next_localisation.pos.y && next_localisation.pos.y<=map.y_max){
            printf("move in bounds\n");
            tmpnode.weight =  map.soils[next_localisation.pos.x][next_localisation.pos.y];
            tmpnode.child_amount = moveset_length-1;
            node->childs[i]=tmpnode;
            if(moves_amount>1){
                printf("prepare recursion\n");
                t_move* new_moveset = removeMoveFromArray(moveset , node->childs[i].move[node->childs[i].movelist_length], moveset_length);
                printf("\n\n\n");
                buildBranch(&(node->childs[i]) , new_moveset , moveset_length-1 , moves_amount-1 , next_localisation , map);
            }
            else{
                printf("out of moves\n\n\n\n");
            }
        }
        else{
            printf("\nout of bounds\n\n\n\n");
        }
    }
}

t_move* removeMoveFromArray(t_move* moveset , t_move move , int moveset_length){
    int i, j;
    t_move* new_moveset = (t_move*)malloc(sizeof(t_move)*moveset_length);
    for(int j = 0 ; j<moveset_length ; j++){
        new_moveset[j] = moveset[j];
    }
    int trouve = 0;
    for (i = 0; i < moveset_length; i++) {
        printf("i = %d\n" , i);
        if (strcmp(getMoveAsString(new_moveset[i]) , getMoveAsString(move))==0) {
            trouve = 1;
            printf("element found\n");
            for (j = i; j < moveset_length - 1; j++) {
                new_moveset[j] = new_moveset[j + 1];
            }
            (moveset_length)--;
            printf("new moveset pointer initialised\n");
            printf("realloc on moveset length: %d\n" , moveset_length);
            new_moveset = (t_move*)realloc(new_moveset, sizeof(t_move)*(moveset_length));
            printf("realloc succesfull\n");
            if (new_moveset == NULL &&moveset_length > 0) {
                exit(EXIT_FAILURE);
            }
            break;
        }
    }
    return new_moveset;
}

void displayMoveset(t_move* moveset , int moveset_length){
    printf("moveset: [ ");
    for(int i=0 ; i<moveset_length-1 ; i++){
        printf("%s , " , getMoveAsString(moveset[i]));
    }
    printf("%s ]\n" , getMoveAsString(moveset[moveset_length-1]));
}

t_tree sampleTree(){
    t_tree sample_tree;
    t_node root;
    srand(time(NULL));
    root.weight=rand()%20;
    printf("root weight = %d\n" , root.weight);
    setChildren(&root , 4);
    t_move* moves = getRandomMoves(4);
    for(int i=0 ; i<4 ; i++){
        t_node child_node;
        child_node.move = (t_move*)malloc(sizeof(t_move));
        child_node.move[0] = moves[i];
        child_node.movelist_length = 1;
        child_node.child_amount=0;
        child_node.weight = (rand()%19)+1;
        printf("%d) child node prepared: weight=%d\n" , i , child_node.weight);
        root.childs[i] = child_node;
        printf("child node assigned\n");
    }
    sample_tree.root = root;
    return sample_tree;
}

void findLowestWeightMove(t_node* node, int* min_weight , t_move** best_move) {
    if (node == NULL) {
        return NULL; 
    }
    printf("child amount = %d\n" , node->child_amount);
    printf("node weight = %d\nmin weight = %d\n" , node->weight , *min_weight);
    if (node->weight < *min_weight) {
        printf("trying to modify pointer\n");
        *min_weight = node->weight;
        best_move = &(node->move); 
        printf("pointer modified\n");
    }
    if(node->child_amount!=0){
        for (int i = 0; i < node->child_amount; i++) {
            printf("i=%d\n" , i);
            findLowestWeightMove(&node->childs[i], min_weight , best_move);
        }
    }
}


t_move* getLowestWeightMove(t_tree* tree) {
    if (tree == NULL || tree->root.child_amount == 0) {
        return NULL; 
    }
    t_move** best_move = NULL;
    printf("initialised best move pointer\n");
    int min_weight = tree->root.weight;
    findLowestWeightMove(&tree->root, &min_weight , best_move);
    return *(best_move);
}