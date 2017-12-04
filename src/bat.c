#include "common.h"

declare_body(move_bat, {
    int cooldown;
    int on_wall;
	int on_floor;
	int is_smart;
})

declare_draw(move_bat, { })
declare_tick(move_bat, {
    
    sprite_renderer *sr = get_component(parent, sprite_renderer);
	entity *floor = get_floor(parent);
	if((parent->position.x < 5) || floor) {
		if(parent->position.y < 10) {
			parent->position = vadd(parent->position, vxy(-1, 0));
			return 0;
		}
		
		if(floor) {
			if(!self->on_floor) {
				set_animator(sr, &sprite_bat_low_bash, 4);
				self->cooldown = 15;
			}
			if(self->cooldown) --self->cooldown;
			else {
				damage(floor, 5);
				play_sound(sound_hit);
				self->cooldown = 20;
			}
			self->on_floor = 1;
		}
		else {
			if(self->on_floor) {
				set_animator(sr, &sprite_bat, 4);
			}
			parent->position = vadd(parent->position, vxy(0, -1));
			self->on_floor = 0;
		}
	}
	else {
		entity *wall = get_wall(parent, self->is_smart);
		if(wall) {
			if(!self->on_wall) {
				set_animator(sr, &sprite_bat_side_bash, 4);
				self->cooldown = 15;
			}
			if(self->cooldown) --self->cooldown;
			else {
				damage(wall, 5);
				play_sound(sound_hit);
				self->cooldown = 20;
			}
			self->on_wall = 1;
		}
		else {
			if(self->on_wall) {
				set_animator(sr, &sprite_bat, 4);
			}
			parent->position = vadd(parent->position, vxy(-1, 0));
			self->on_wall = 0;
		}
	}
})

declare_head(move_bat);

entity *create_bat() {
    entity *e = ent_new();
    e->position = vxy(500, get_random_y());
    e->tag = ENEMY;
    e->layer = 1;
    
    move_bat *mv = c_new(move_bat);
    mv->cooldown = 20;
	mv->is_smart = rand() % 2;
    add_component(e, mv);
    
    add_component(e, get_enemy_health());
    add_component(e, get_animator(&sprite_bat, 4));
    add_component(e, get_health(30));
    
    return e;
}

declare_body(move_arrow_bat, {
})

declare_draw(move_arrow_bat, { })
declare_tick(move_arrow_bat, {
    sprite_renderer *sr = get_component(parent, sprite_renderer);
    if(parent->position.x >= 310) {
        parent->position = vadd(parent->position, vxy(-1, 0));
    }
})

declare_head(move_arrow_bat);

entity *create_bow_bat() {
    entity *e = ent_new();
    e->position = vxy(500, get_random_y());
    e->tag = ENEMY;
    e->layer = 1;
    
    move_arrow_bat *mv = c_new(move_arrow_bat);
    add_component(e, mv);
    
    add_component(e, get_enemy_health());
    add_component(e, get_animator(&sprite_bat, 4));
    add_component(e, get_animator_c(&sprite_spider_bow, 4, vxy(0, 3)));
    add_component(e, get_health(8));
    
    shoot_gun *sg = c_new(shoot_gun);
    sg->cooldown = 30;
    sg->last_shot = 20;
    sg->shoot = &create_enemy_arrow;
    add_component(e, sg);
    
    return e;
}

declare_body(move_bomb_bat, {
})

declare_draw(move_bomb_bat, { })
declare_tick(move_bomb_bat, {
    entity *wall = ent_first(within_x_range(parent->position.x, 32), within_y_range(parent->position.y, 1), tagged(TOWER), is_not(with_component(c_type(smasher_flag))));
    if(wall) {
        create_explosion(parent->position);
        for_ent(e, ent_all(
                    tagged(TOWER),
                    within_radius(parent->position, 200)
                ), {
                    float dmg = (1- (len(vsub(parent->position, e->position)) / 200)) * 50;
                    damage(e, dmg);
                })
        return 1;
    }
    else {
        parent->position = vadd(parent->position, vxy(-6, 0));
    }
})

declare_head(move_bomb_bat);

entity *create_bomb_bat() {
    entity *e = ent_new();
    e->position = vxy(500, get_random_y());
    e->tag = ENEMY;
    e->layer = 1;
    
    add_component(e, c_new(move_bomb_bat));
    add_component(e, get_animator(&sprite_bat, 4));
	add_component(e, get_animator_c(&sprite_spider_bomb, 4, vxy(-6, -8)));
    enemy_health *eh = get_enemy_health();
    eh->yoff = 13;
    add_component(e, eh);
    add_component(e, get_health(20));
    
    return e;
}

declare_body(move_shield_bat, {
    int on_wall;
})

declare_draw(move_shield_bat, { })
declare_tick(move_shield_bat, {
    entity *wall = get_wall(parent, 1);
    sprite_renderer *sr = get_component(parent, sprite_renderer);
    if(wall) {
		self->on_wall = 1;
    }
    else {
        parent->position = vadd(parent->position, vxy(-0.9f, 0));
        self->on_wall = 0;
    }
})

declare_head(move_shield_bat);

entity *create_shield_bat() {
    entity *e = ent_new();
    e->position = vxy(500, get_random_y());
    e->tag = ENEMY;
    e->layer = 1;
    
    move_shield_bat *mv = c_new(move_shield_bat);
    add_component(e, mv);
    
    add_component(e, get_enemy_health());
    add_component(e, get_animator(&sprite_bat_shield, 4));
    add_component(e, get_health(50));
    
    return e;
}

declare_body(move_spear_bat, {
    int cooldown;
    int on_wall;
})

declare_draw(move_spear_bat, { })
declare_tick(move_spear_bat, {
    entity *wall = ent_first(within_x_range(parent->position.x, 32), within_y_range(parent->position.y, 1), tagged(TOWER), is_not(with_component(c_type(smasher_flag))));
    sprite_renderer *sr = get_component(parent, sprite_renderer);
    if(wall) {
        if(!self->on_wall) {
            set_animator_c(sr, &sprite_bat_spear_bash, 4, vxy(5, 0));
            self->cooldown = 15;
        }
        if(self->cooldown) --self->cooldown;
        else {
            damage(wall, 7);
			play_sound(sound_hit);
            self->cooldown = 20;
        }
        self->on_wall = 1;
    }
    else {
        if(self->on_wall) {
            set_animator(sr, &sprite_bat_spear, 4);
        }
        parent->position = vadd(parent->position, vxy(-1.5f, 0));
        self->on_wall = 0;
    }
})

declare_head(move_spear_bat);

entity *create_spear_bat() {
    entity *e = ent_new();
    e->position = vxy(500, get_random_y());
    e->tag = ENEMY;
    e->layer = 1;
    
    move_spear_bat *mv = c_new(move_spear_bat);
    mv->cooldown = 20;
    add_component(e, mv);
    
    add_component(e, get_enemy_health());
    add_component(e, get_animator(&sprite_bat_spear, 4));
    add_component(e, get_health(25));
    
    return e;
}