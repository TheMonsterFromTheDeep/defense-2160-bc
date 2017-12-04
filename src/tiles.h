#ifndef TILES_H
#define TILES_H

#include "entity.h"
#include "button.h"

#include "shoehorn_sdl.h"

extern sound* sound_build;
extern sound* sound_bow;
extern sound* sound_clicking;
extern sound* sound_hit;
extern sound* sound_cannon;
extern sound* sound_explosion;
extern sound* sound_wave_start;
extern sound* sound_clock;
extern sound* sound_spear;

enum tags {
    ENEMY,
    TOWER
};

entity *create_gun(vec);
entity *create_shield(vec);
entity *create_arrow_launcher(vec);
entity *create_core();
entity *create_smasher(vec);
entity *create_spear_launcher(vec);

entity *create_tower_selector();
entity *create_price();
entity *create_next_wave();
entity *create_money_visual();

entity *create_land(float);

entity *create_cannonball(vec);
entity *create_arrow(vec);

entity *create_explosion(vec);

entity *create_spider();
entity *create_bow_spider();
entity *create_bomb_spider();
entity *create_shield_spider();
entity *create_spear_spider();

entity *create_snake();
entity *create_bow_snake();
entity *create_bomb_snake();
entity *create_shield_snake();
entity *create_spear_snake();

entity *create_bat();
entity *create_bow_bat();
entity *create_bomb_bat();
entity *create_shield_bat();
entity *create_spear_bat();

void spawn_spider();
void spawn_snake();

void enemy_spawn();

entity *create_selector();

void init_spawns();

#endif