#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"
#include "tree.h"
#include "loc.h"
#include "node.h"
#include "queue.h"

t_tree buildTree( int , int , t_localisation , t_map);
void buildBranch( t_node* , t_move* , int , int , t_localisation , t_map);
t_move* buildRandomMovesArray(int);
t_move* removeMoveFromArray(t_move* , t_move , int);
void displayMoveset(t_move* , int);


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
    displayMap(map);
    return 0;
}

t_move* buildRandomMovesArray(int array_length){
    t_move moveset[7] = { F_10, F_20, F_30, B_10, T_LEFT, T_RIGHT, U_TURN };
    t_move* result;
    srand(time(NULL));
    result= (t_move*)malloc(array_length*sizeof(t_move));
    for(int i = 0; i<array_length ; i++){
        result[i]=moveset[rand()%array_length];
    }
    return result;
}

t_tree buildTree(int moveset_length , int moves_amount , t_localisation localisation , t_map map){
    t_tree tree;
    t_node root;
    tree.root = root;
    tree.root.child_amount = moveset_length;
    tree.root.movelist_length = 0;
    tree.root.weight = map.soils[localisation.pos.x][localisation.pos.y];
    t_move* moveset = buildRandomMovesArray(moveset_length);
    buildBranch(&(tree.root) , moveset , moveset_length , moves_amount , localisation , map);
    return tree;
}

void buildBranch(t_node* node , t_move* moveset , int moveset_length , int moves_amount , t_localisation localisation , t_map map){
    
    setChildren(node , moveset_length);
    for(int i=0 ; i<moveset_length ; i++){
        printf("\n\n\n");
        displayMoveset(moveset , moveset_length);
        printf("moveset length: %d\n" , moveset_length);
        t_node tmpnode;
        printf("node initilized\n");
        t_move* node_move;
        node_move = (t_move*)malloc(sizeof(t_move)*node->movelist_length+1);
        for(int j=0 ; j<node->movelist_length ; j++){
            node_move[j]=node->move[j];
        }
        printf("position: x=%d y=%d\n" , localisation.pos.x , localisation.pos.y);
        node_move[node->movelist_length+1]=moveset[i];
        printf("move from moveset chosen: %s\n" , getMoveAsString(node_move[node->movelist_length+1]));
        tmpnode.move = node_move;
        t_localisation next_localisation = move(localisation , *(node_move));
        if(0<=next_localisation.pos.x && next_localisation.pos.x<=map.x_max && 0<=next_localisation.pos.y && next_localisation.pos.y<=map.y_max){
            tmpnode.weight =  map.soils[next_localisation.pos.x][next_localisation.pos.y];
            tmpnode.child_amount = moveset_length-1;
            tmpnode.movelist_length=node->movelist_length+1;
            node->childs[i]=tmpnode;
            printf("new node: weight = %d , move = %s\n" , node->childs[i].weight , getMoveAsString(node->childs[i].move[node->childs[i].movelist_length]));
            printf("moves amount = %d\n" , moves_amount);
            if(moves_amount>1){
                printf("coucou");
                t_move* new_moveset = removeMoveFromArray(moveset , node->childs[i].move[node->childs[i].movelist_length], moveset_length);
                printf("c'est moi");
                displayMoveset(new_moveset , moveset_length-1);
                buildBranch(&(node->childs[i]) , new_moveset , moveset_length-1 , moves_amount-1 , next_localisation , map);
            }
        }
        else{
            printf("\nout of bounds\n");
        }
    }
}

int getSmallestWeight(t_tree tree){
    int smalless = tree.root.weight;
}

t_move* removeMoveFromArray(t_move* moveset , t_move move , int moveset_length){
    int i, j;
    int trouve = 0;

    // Parcourir la liste pour trouver la valeur à supprimer
    for (i = 0; i < moveset_length; i++) {
        printf("i = %d\n" , i);
        if (getMoveAsString(moveset[i]) == getMoveAsString(move)) {
            trouve = 1;
            printf("trouvé");
            // Décaler les éléments suivants vers la gauche
            for (j = i; j < moveset_length - 1; j++) {
                moveset[j] = moveset[j + 1];
            }
            // Réduire la taille de la liste
            (moveset_length)--;
            printf("realloc on moveset length: %d\n" , moveset_length);
            moveset = (t_move*)realloc(moveset , sizeof(t_move)*moveset_length);
            break;
        }
    }
    printf("je vais me buter");
    return moveset;
}

void displayMoveset(t_move* moveset , int moveset_length){
    printf("moveset: [ ");
    for(int i=0 ; i<moveset_length-1 ; i++){
        printf("%s , " , getMoveAsString(moveset[i]));
    }
    printf("%s ]\n" , getMoveAsString(moveset[moveset_length-1]));
}