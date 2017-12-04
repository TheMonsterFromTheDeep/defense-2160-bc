#ifndef COMMON_H
#define COMMON_H

#include "entity.h"
#include "button.h"

#include "shoehorn_sdl.h"

#include "tiles.h"
#include "graphics.h"
#include "images_diffuse.h"
#include "particle.h"

#include <stdlib.h>
#include <math.h>

typedef enum {
	MENU,
	HELP,
	GAME,
	NONE,
	LOSE,
	WIN
} mode_k;

extern mode_k switch_to;

declare_body(core, { })

declare_body(smasher_flag, {
    int tick;
})

declare_body(health, {
    float amount;
    float visual;
    float total;
})
void damage(entity*,float);
health* get_health(float);

declare_body(tower_health, { })

declare_body(enemy_health, { float xoff; float yoff; })
enemy_health *get_enemy_health();

declare_body(healer, {
    float rate;
})
healer *get_healer(float);

declare_body(shoot_gun, {
    int cooldown;
    int last_shot;
    entity* (*shoot)(vec);
})
entity *create_arrow(vec);
entity *create_cannonball(vec);
entity *create_enemy_arrow(vec);

declare_body(particle_killer, { int life; })
particle_killer *get_particle_killer(int);

declare_body(number_renderer, {
	int *value;
	float xoff;
	float yoff;
})

float get_random_y();
entity *get_wall(entity*,int);
entity *get_floor(entity*);

#define TOWER_COUNT 5

extern int total_money;
extern int prices[TOWER_COUNT];

typedef entity* (*create_tower)(vec);
extern create_tower towers[TOWER_COUNT];

void refund(entity*);
int full_refund(entity*);

typedef entity* (*enemy_spawner)(void);
extern zlist_of(enemy_spawner) spider_types;
extern zlist_of(enemy_spawner) snake_types;
extern zlist_of(enemy_spawner) bat_types;

declare_body(hold_upgrade, {
    enemy_spawner spider;
	enemy_spawner snake;
	enemy_spawner bat;
})

hold_upgrade *upgrader(enemy_spawner, enemy_spawner, enemy_spawner);

typedef struct {
	int spider_quota;
	int spider_rate;
	int snake_quota;
	int snake_rate;
	int bat_quota;
	int bat_rate;
} wave;

void init_game_values();

#define WAVE_COUNT 34
extern wave waves[WAVE_COUNT];

#endif