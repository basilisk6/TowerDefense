#ifndef _SETUP_H_
#define _SETUP_H_

#define PI 3.141592
#define EPS 0.0001

extern int reshapeWidth, reshapeHeight;
extern int readyFlag, animation_ongoing;
extern float orr, ol, ob, ot;
extern float thet, phi, targetThet, targetPhi;
extern float posix, posiy;
extern int minion_count;

/* Function for light enabling */
void lights(void);
/* Reshape function */
void reshape(int width, int height);
/* Draw/display function */
void display(void);
/* Keyboard function */
void keyboard(unsigned char key, int x, int y);
/* Timer frunctions which move the position of camera and starts the game */
void cam_move(int value);
void game_mode(int value);
/* Mouse function */
void mouse(int button, int state, int x, int y);

#endif