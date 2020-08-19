#ifndef _MODEL_H_
#define _MODEL_H_

#define PI 3.141592
#define EPS 0.0001

/* Minion structure */
typedef struct{
    float x, y;
    int target;
    int alive;
    float health;
}minion;
/* Minion initialization */
void minion_init(minion* m);
/* Function that draws minion */
void draw_minion(const minion *m);
/* Function that adjusts the position of minion while moving through path */
void rotate_minion(const minion *m);
/* Function that enables towers to deal damage to minions */
void deal_damage_to_minion(minion *m, float damage);

/* Function for drawing terrain */
void draw_terrain(void);
/* Function for drawing towers */
void draw_tower(float posX, float posY);
/* Texturing the walls of a tower */
void setup_wall_texture(const char* name);
/* Function for bullet drawing */
void draw_bullet(float towX, float towZ, float minX, float minY);
/* Help function for deciding how much range will towers have */
void draw_range(float x, float z);

#endif