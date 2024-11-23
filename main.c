#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"
#include "tree.h"
#include "loc.h"
#include "node.h"

t_tree buildTree( int , int , t_localisation , t_map);
t_tree buildBranch( t_node* , t_move* , int , int , t_localisation , t_map);
t_move* buildRandomMovesArray(int);
t_move* removeMoveFromArray(t_move* , t_move , int);


int main() {
    t_map map;
    t_localisation rover;
    // The following preprocessor directive checks if the code is being compiled on a Windows system.
    // If either _WIN32 or _WIN64 is defined, it means we are on a Windows platform.
    // On Windows, file paths use backslashes (\), hence we use the appropriate file path for Windows.
#if defined(_WIN32) || defined(_WIN64)
    map = createMapFromFile("..\\maps\\example1.map");
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
                buildTree(5 , 3 , rover , map);
            }
        }
        printf("\n");
    }
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
    t_move* moveset = buildRandomMovesArray(moveset_length);
    buildBranch(&(tree.root) , moveset , moveset_length , moves_amount , localisation , map);
}

t_tree buildBranch(t_node* node , t_move* moveset , int moveset_length , int moves_amount , t_localisation localisation , t_map map){
    if(moveset_length!=0){
        setChildren(node , moves_amount);
        for(int i=0 ; i<moveset_length ; i++){
            t_node tmpnode;
            t_move node_move = moveset[rand()%moveset_length]; 
            tmpnode.move = node_move;
            t_localisation next_localisation = move(localisation , node_move);
            tmpnode.weight =  map.soils[next_localisation.pos.x][next_localisation.pos.y];
            node->childs[i]=tmpnode;
            printf("\n moveset length: %d" , moveset_length);
            printf("\nnew moveset generated");
            buildBranch(&(node->childs[i]) , removeMoveFromArray(moveset , node_move , moveset_length) , moveset_length-1 , moves_amount-1 , next_localisation , map);
        }
    }
}

t_move* removeMoveFromArray(t_move* moveset , t_move move , int moveset_length){
    if(moveset_length>1){
        t_move* new_moveset = (t_move*)malloc(sizeof(t_move)*moveset_length-1);
        int counter = 0;
        int flag = 0;
        printf("\n\n{new moveset prepared");
        for(int i=0 ; i<moveset_length ; i++){
            printf("\ni = %d ; counter = %d" , i , counter);
            if(moveset[i]!=move||flag>=1){
                new_moveset[counter]=moveset[i];
                counter++;
            }
            if(moveset[i]==move){
                flag++;
            }
        }
        printf("\ndeleted %s from moveset}\n" , getMoveAsString(move));
        return new_moveset;
    }
    return NULL;
}
