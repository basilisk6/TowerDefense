#include "grid.h"
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

int grid_representation[10][10];

/* Representation of grid */
void init_grid(void){
    for(int i = 0; i < 10; i++){
	    for(int j = 0; j < 10; j++){
       	    	grid_representation[i][j] = 0;
   	    }
    } 
}

/* Grid function */
void draw_grid(void){
	glEnable(GL_COLOR_MATERIAL);
    glNormal3f(1, 1, 0);
	glColor3f(1, 1, 1);
	for(int i = -5; i <= 5; i++){
		glBegin(GL_LINES);
			glVertex3f(i, 0, 5);
			glVertex3f(i, 0, -5);
		glEnd();
	}
	for(int i = -5; i <= 5; i++){
		glBegin(GL_LINES);
			glVertex3f(-5, 0, i);
			glVertex3f(5, 0, i);
		glEnd();	
	}
	glDisable(GL_COLOR_MATERIAL);
}

/* Coord helper */
void draw_cords(){
	
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1,0,0);
	glBegin(GL_LINE_STRIP);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
	glEnd();
	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);
		glVertex3f(0,0,0);
		glVertex3f(0,10,0);
	glEnd();
	glColor3f(0,0,1);
	glBegin(GL_LINE_STRIP);
		glVertex3f(0,0,0);
		glVertex3f(0,0,10);
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
}

/* Function that updates matrix of the grid */
void add_to_matrix(float x, float y){
	int indx = x;
	int indy = fabs(y);

	if(grid_representation[indy][indx] != 1){
		grid_representation[indy][indx] = 1;
	}
}
