#include "movement.h"
#include "model.h"
#include "setup.h"
#include "grid.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <IL/il.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

int graph_representation[100][100], begin = 0, finish = 99,  count = 1, path[100];

/* Function that represents available path */
void map_graph(void){
    for(int i = 0; i < 100; i++){
        for(int j = 0 ; j < 100; j++){
            graph_representation[i][j] = 0;
        }
    }

    for(int i = 0; i < 100; i++){
        for(int j = 0; j < 100; j++){
            graph_representation[i][j+i+1] = 1;
            graph_representation[i][j+i-1] = 1;
            break;
        }
    }

    for(int i = 10; i < 100; i++){
        for(int j = 0; j < 100; j++){
            graph_representation[i][i-10] = 1;
            graph_representation[i-10][i] = 1;
            break;
        }
    }

    for(int i = 9; i < 100; i+=10){
        graph_representation[i][i+1] = 0;
        graph_representation[i+1][i] = 0;
    }

    /* Binding grid representation with towers onto map path - disconnecting paths where towers are placed */
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(grid_representation[i][j] == 1){
                int jk = i*10 + j;
                for(int k = 0; k < 100; k++){
                    graph_representation[jk][k] = 0;
                    graph_representation[k][jk] = 0;
                }
            }
        }
    }
}

/* Function for getting the next node in Dijkstra's algorithm */
int get_next_node(int distance[], int selected[]){
    int min = 101;
    int node_index;

    for(int i = 0; i < 100; i++){
        if(selected[i] == 0 && distance[i] <= min){
            min = distance[i];
            node_index = i;
        }
    }

    selected[node_index] = 1;
    return node_index;
}


/* Dijkstra's algorithm for finding the shortest path available to minions */
void dijkstra(int graph[][100], int start, int finish, int parents[100]){

    /* Initialization */
    int distance[100], selected[100];
    int current_node = start, neighbour_node;
    for(int i = 0;i < 100;i++){
        distance[i] = 101;
        selected[i] = 0;
    }
    distance[start] = 0;

    /* Search */
    while(current_node != finish){
        current_node = get_next_node(distance, selected);
        for(int i = 0; i < 100; i++){
            if(current_node == finish){
                /* Help */
                //printf("Found a way\n");
                return;
            }
            if(graph[current_node][i] == 1){
                neighbour_node = i;
                if(distance[neighbour_node] > distance[current_node]+1 && selected[neighbour_node] == 0){
                    distance[neighbour_node] = distance[current_node] + 1;
                    /* Help */
                    //fprintf(stderr, "For node %d found shorter path through node %d\n", neighbour_node, current_node);
                    parents[neighbour_node] = current_node;
                }
            }
        }
    }
}

void construct_path(){
    /* Getting the length of path */
    for(int j = finish;;){
        count++;
        j = parents[j];
        if(j == begin){
            break;
        }
	}

    /* Inverting the parents and setting the path (First parent is last in the path) */
    for(int i = count-1, j = finish; i >= 0; i--){
        path[i] = j;
        j = parents[j];
    }

    /* Help */
    //for(int i = 0; i < count; i++){
    //    printf("%d ", path[i]);
    //}
    //printf("\n");
}

/* Transforms the field in the grid into the world x/y coordinates */
float field_x(int field){
    int col = field % 10;
    return -4.5+col;
}
float field_y(int field){
    int row = field / 10;
    return -4.5+row;
}

/* Initializing starting and finish positions */
void init_start_finish_position(void){
    /* Randomizing */
    int side_begin = rand() % 4;
    int position_begin = rand() % 10;
    int side_end = -1;
    int position_end = -1;

    if(side_begin == 0){
        begin = position_begin;
    } else if(side_begin == 2){
        begin = 90 + position_begin;
    } else if(side_begin == 1){
        begin = position_begin*10 + 9;
    } else if(side_begin == 3){
        begin = position_begin*10;
    }

    do{
        side_end = rand() % 4;
        position_end = rand() % 10;

        if(side_end == 0){
            finish = position_end;
        } else if(side_end == 2){
            finish = 90 + position_end;
        } else if(side_end == 1){
            finish = position_end*10 + 9;
        } else if(side_end == 3){
            finish = position_end*10;
        }

    }while(begin == finish || side_end == side_begin || 
            fabs(pow(field_x(finish) - field_x(begin), 2) + pow(field_y(finish) - field_y(begin),2)-1) < EPS );
            //Resolving the corner cases like overlapping the tiles or being start-finish tiles neighbours
    
    /* Help */
    //printf("%d %d \n", begin, finish);
}

/* Function that checks if selected tower can shoot the specific minion */
int can_shoot(float posTowX, float posTowY, const minion* m){
    posTowX = floor(posTowX) + 0.5;
    posTowY = floor(posTowY) + 0.5;

    for(int i = 0; i < minion_count; i++){
        if(m[i].alive){
            float len = sqrt(pow(posTowX - m[i].x, 2) + pow(posTowY - m[i].y, 2));
            if(len < 1.5 + EPS){
                return i;
            } 
        }
    }
    return -1;
}
