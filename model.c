#include "model.h"
#include "movement.h"
#include <math.h>
#include <IL/il.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

unsigned wall_tex;
extern int begin, finish;

/* Terrain function */
void draw_terrain(void){
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0, 0.6, 0.8); //TODO: change color into texture 
	
    glBegin(GL_QUADS);
        glVertex3f(17, -7, 17);
		glVertex3f(17, -7, -17);
		glVertex3f(-17, -7, -17);
		glVertex3f(-17, -7, 17);
	glEnd();

    glColor3f(0, 1, 0);
    glPushMatrix();
        glTranslatef(field_x(begin), -0.1, field_y(begin));
        glScalef(1, 0.1, 1);
        glutSolidCube(1);
    glPopMatrix();


    glColor3f(1, 0, 0);
    glPushMatrix();
        glTranslatef(field_x(finish), -0.1, field_y(finish));
        glScalef(1, 0.1, 1);
        glutSolidCube(1);
    glPopMatrix();


	glDisable(GL_COLOR_MATERIAL);
}

/* Tower function */
void draw_tower(float posX, float posY){
	posX = floor(posX) + 0.5;
	posY = floor(posY) + 0.5;

	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1,0,0);

	/* Rooftop */
	glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		glTranslatef(posX, posY, 1);
		glutSolidCone(0.225, 0.5, 20, 20);
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);

	/* Body/Walls */
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, wall_tex);
    glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		glTranslatef(posX, posY, 0);
		GLUquadricObj *cylinder = gluNewQuadric();
        
        /* Initalizing texture */
        gluQuadricNormals(cylinder, GLU_SMOOTH);
        gluQuadricTexture(cylinder, GLU_TRUE);
        
        gluCylinder(cylinder, 0.2, 0.2, 1, 20, 20);
	glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
}

/* Minion function */
void draw_minion(const minion *m){
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(0, 1, 0.2);

    glPushMatrix();
        glTranslatef(m->x, 0, m->y);
        rotate_minion(m);
        glScalef(0.3, 0.3, 0.3);

        glPushMatrix();
            glNormal3f(1, 0, 1);
            glLineWidth(2);
            glTranslatef(-(PI/2)-0.8, 0, 0);
            
            /* Tail */
            glBegin(GL_LINE_STRIP);
                for(float i = 0; i < PI/2+0.5 + EPS; i+=PI/360){
                    glVertex3f(i, 1, sin(i*7)/1.3);
                }
            glEnd();
            
            glLineWidth(1);
        glPopMatrix();

        glTranslatef(0, 0.5, 0);
        /* Body */
        glutSolidSphere(1, 20, 20);
        glTranslatef(0.7, 0.6, 0);
        /* Head */
        glutSolidSphere(0.5, 20, 20);
    glPopMatrix();

    glDisable(GL_COLOR_MATERIAL);
}

/* Setting texture for wall */
void setup_wall_texture(const char* name){
    ILuint wall;
    
    ilGenImages(1, &wall);
    ilBindImage(wall);
    ilLoadImage(name);

    /* Enabling and binding texture */
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &wall_tex);
    glBindTexture(GL_TEXTURE_2D, wall_tex);

    /* Parameters of textures */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH),
                                ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());

    glDisable(GL_TEXTURE_2D);

    ilDeleteImages(1, &wall);
}

/* Function for minion initialization */
void minion_init(minion* m){
    /* Coordinates */
    m->x = field_x(begin);
    m->y = field_y(begin);
    /* Target is the index of a field in a path. Minons move by using the indicator of the next target */
    m->target = 1;
    /* In game stats */
    m->alive = 0; 
    m->health = 100;
}

/* Function that adjusts the position of minion while moving through path */
void rotate_minion(const minion *m){
    /* They are rotated based on the recent and current value in path. Depending on that, they rotate and continue moving */
    if(path[m->target-1] - path[m->target] == -10){
        glRotatef(-90, 0, 1, 0);
    } else if(path[m->target-1] - path[m->target] == 10){
        glRotatef(90, 0, 1, 0);
    } else if(path[m->target-1] - path[m->target] > 0){  
        glRotatef(180, 0, 1, 0);
    }
}

/* Function for bullet drawing */
void draw_bullet(float towX, float towY, float minX, float minY){
    towX = floor(towX) + 0.5;
	towY = floor(towY) + 0.5;

    glPushMatrix();
        glEnable(GL_COLOR_MATERIAL);
        glColor3f(1, 1, 0);
            glLineWidth(10);
            glBegin(GL_LINES);
                glVertex3f(towX, 1, towY);
                glVertex3f(minX, 0.2, minY);
            glEnd();
        glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
}

/* Help function for deciding how much range will towers have */
void draw_range(float x, float z){
    x = floor(x) + 0.5;
	z = floor(z) + 0.5;

    glPushMatrix();
    glTranslatef(x, 0, z);
    glColor3f(1, 1, 0);
        glBegin(GL_LINE_STRIP);
            for(float i = 0; i < 2*PI + EPS; i+= PI/20){
                glVertex3f(1.5*cos(i), 0, 1.5*sin(i));
                glVertex3f(1.5*cos(i+PI/20), 0, 1.5*sin(i+PI/20));
            }
        glEnd();

    glPopMatrix();
}

/* Function that enables towers to deal damage to minions */
void deal_damage_to_minion(minion *m, float damage){
    m->health -= damage;
    if(m->health <= 0){
        m->alive = 0;
    }

}