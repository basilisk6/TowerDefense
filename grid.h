#ifndef _GRID_H_
#define _GRID_H_

extern int grid_representation[10][10];

/* Help function for orientating in space */
void draw_cords(void);
/* Function that draws grid */
void draw_grid(void);
/* Function that initializes the grid representation that is later used in tower placing */
void init_grid(void);
/* Function that updates matrix of the grid if you add the tower */
void add_to_matrix(float x, float y);

#endif