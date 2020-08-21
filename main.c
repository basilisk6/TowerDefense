#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <IL/il.h>
#include "movement.h"
#include "setup.h"
#include "grid.h"
#include "model.h"

extern int begin, finish;
int parents[100], minion_count = 10, minions_left = 10;
minion m[10];

int main(int argc, char* argv[]){

	/* Setting the scene */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(600, 400);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Tower defense");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	srand(time(NULL));
	ilInit();
	setup_wall_texture("wall.jpg");

	/* Enabling scene preferences */
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
	
    init_grid();
	init_start_finish_position();
	init_parents();
	for(int i = 0; i < minion_count; i++){
		minion_init(m+i);
	}

	glutMainLoop();

	return 0;
}

/* Display function */	
void display(void){
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Setting perspective */
	if(readyFlag == 0){
		/* Bird perspective */
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(ol, orr, ob, ot, -2, 40);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(90, 1, 0, 0);
	} else {
		/* In game perspective */
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, (float) reshapeWidth/reshapeHeight, 1, 40);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(15.5*sin(thet)*sin(phi), 15.5*cos(thet), 15.5*sin(thet)*cos(phi), 0, 0, 0, 0, 1, 0);
	}

	/* Help */
	//draw_cords();
	/* Enabling lights */
	lights();
	/* Drawing grid */
	draw_grid();
	/* Setting terrain */
	draw_terrain();

	/* Setting towers and targets */
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			if(grid_representation[i][j] != 0){
				/* Tower drawing */
				draw_tower(j-5, -(i-4));
				/* Help */
				//draw_range(j-5, i-5);
				/* Choosing minion for shooting */
				int target = can_shoot(j-5, i-5, m);
				/* If the target is alive, shoot!*/
				if(target != -1){
					draw_bullet(j-5, i-5, m[target].x, m[target].y);
					deal_damage_to_minion(&m[target], 0.1);
				}
			}
		}
	}

	/* Dropping creeps to map */
	for(int i = 0; i < minion_count; i++){
		if(m[i].alive){
			draw_minion(m+i);
		}
	}

	glutSwapBuffers();
}

/* Function for initializing parents of a node (Later used for Dijsktra's algorithm) */
void init_parents(void){
	for(int i = 0; i < 100; i++){
		parents[i] = -1;
	}
}


