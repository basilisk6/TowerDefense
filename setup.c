#include "setup.h"
#include "model.h"
#include "movement.h"
#include "grid.h"
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>

int reshapeWidth, reshapeHeight;
float orr, ol, ob, ot;
int readyFlag = 0, animation_ongoing = 0, spawn_time = 50, spawn_position = 0;
float thet = 0 + EPS, phi = EPS, targetThet = PI/4, targetPhi = PI/4;
float posix, posiy, mvmnt = -5;
extern minion m[10];
extern int minion_count, minions_left;

/* Lights function */
void lights(void){
	
	/* Light position */
	GLfloat light_position[] = { 1, 1, 1, 0};
    
    /* Object light */
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1 };
    GLfloat ambient_coeffs[] = { 0, 0, 0, 1 };

    /* From-point light */
    GLfloat light_diffuse[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4, 1 };
    
    /* From-point light */
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
    
    GLfloat shininess = 40;

    glEnable(GL_LIGHT0);

    /* Setup */
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);
    glutPostRedisplay();
}

/* Reshape function */
void reshape(int width, int height){
	glViewport(0, 0, width, height);

	reshapeWidth = width;
	reshapeHeight = height;

    /* Ortogonal borders */
	ol = -9*reshapeWidth/reshapeHeight;
	orr = 9*reshapeWidth/reshapeHeight;
	ot = 6*reshapeWidth/reshapeHeight;
	ob = -6*reshapeWidth/reshapeHeight;
}


/* Keyboard function */
void keyboard(unsigned char key, int x, int y){
	(void)x;
	(void)y;
	switch(key){
		/* Escape as an exit key */
		case 27:
			exit(0);
			break;
		case 'r':
			glutMouseFunc(NULL);

			/* Representing movement graph for minions */
			map_graph();	
			dijkstra(graph_representation, begin, finish, parents);
			/* Constructing a path */
			construct_path();

			if(!animation_ongoing){
				readyFlag = 1;
				glutTimerFunc(20, cam_move, 0);
				animation_ongoing = 1;
			}
			glutPostRedisplay();
			break;
	}
}

/* Start - moving camera to in-game perspective */
void cam_move(int value){
	if(value != 0){
		return;
	}

	if(thet < targetThet + EPS){
		thet += PI/150;
	}

	if(phi < targetPhi + EPS){
		phi += PI/150;
	}

	if(thet < targetThet + EPS || phi < targetPhi + EPS){
		glutPostRedisplay();
		glutTimerFunc(20, cam_move, 0);
	} else {
		glutPostRedisplay();
		glutTimerFunc(20, game_mode, 1);
	}
}


/* Starting the game - movement of minions */
void game_mode(int value){
	if(value != 1){
		return;
	}

	/* Spawning additional minnions - depending on the time interval */
	if(spawn_position < minion_count){
		spawn_time--;
		if(spawn_time < 0){
			m[spawn_position].alive = 1;
			spawn_time = 50;
			spawn_position++;
		}
	}

	/* Movement of a minion */
	for(int i = 0; i < minion_count; i++){
		if(m[i].alive){
			/* Target is index of field in path */
			if(path[m[i].target-1] - path[m[i].target] == -10){
				m[i].y += 0.05;
				if(fabs(m[i].y - field_y(path[m[i].target])) < EPS){
					m[i].target++;
				}
			} else if(path[m[i].target-1] - path[m[i].target] == 10){
				m[i].y -= 0.05;
				if(fabs(m[i].y - field_y(path[m[i].target])) < EPS){
					m[i].target++;
				}
			} else if(path[m[i].target-1] - path[m[i].target] < 0){
				m[i].x += 0.05;
				if(fabs(m[i].x - field_x(path[m[i].target])) < EPS){
					m[i].target++;
				}
			} else if(path[m[i].target-1] - path[m[i].target] > 0){
				m[i].x -= 0.05;
				if(fabs(m[i].x - field_x(path[m[i].target])) < EPS){
					m[i].target++;
				}
			}

			/* End condition */
			if(path[m[i].target-1] == finish){
				m[i].alive = 0;
				minions_left = 0;
				printf("G A M E O V E R \n");
			}

		}
	}
	
	if(minions_left != 0){
		glutPostRedisplay();
		glutTimerFunc(20, game_mode, 1);
	}
}

/* Mouse function */
void mouse(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){		
		/* Scaling map coordinates */	
		posix = (float)(orr-ol)/reshapeWidth*x+ol;
		posiy = (float)(ob-ot)/reshapeHeight*y+ot;
						
       	/* Disabling making towers out of boundaries */
		if(posix < -5 || posix > 5 || posiy < -5 || posiy > 5){
			return;
		}

		/* Loading tower into grid matrix */
		add_to_matrix(posix+5, posiy-5);		
        
		/* Help */
        //printf("%f %f\n", posix, posiy);

		/*
		for(int i = 0; i < 10; i++){
    		for(int j = 0; j < 10; j++){
    			printf("%d ", grid_representation[i][j]);
    		}
    		printf("\n");
    	}
		*/

		/*
		for(int i = 0; i < 100; i++){
			for(int j = 0; j < 100; j++){
				printf("%d", graph_representation[i][j]);
			}
			printf("\n");
		}
		*/
	}
}


