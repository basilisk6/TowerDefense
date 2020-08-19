#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_
#include "model.h"

extern int graph_representation[100][100];
extern int parents[100], begin, finish, count, path[100];

/* Function that makes graph representation of available path */
void map_graph(void);
/* Function that finds the shortest path available using Dijkstra's algorithm */
void dijkstra(int graph[][100], int start, int finish, int parents[100]);
/* Function for initializing parents of a node - used in Dijkstra */
void init_parents(void);
/* Function that uses parents which we got from Dijkstra and represents them in regular order for movement of creeps */
void construct_path(void);
/* Transforms the field in the grid into the world x/y coordinates */
float field_x(int field);
float field_y(int field);
/* Initializes starting and finish positions of a minions */
void init_start_finish_position(void);
/* Function that checks if selected tower can shoot the specific minion */
int can_shoot(float posTowX, float posTowY, const minion *m);

#endif