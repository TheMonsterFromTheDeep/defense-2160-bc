#include "common.h"

declare_body(move_snake, {
    int cooldown;
    int on_wall;
})

declare_draw(move_snake, { })
declare_tick(move_snake, {
    entity *wall = ent_first(within_x_range(parent->position.x, 32), within_y_range(parent->position.y, 1), tagged(TOWER));
    sprite_renderer *sr = get_component(parent, sprite_renderer);
    if(wall) {
        if(!self->on_wall) {
            set_animator(sr, &sprite_snake_bash, 6);
            self->cooldown = 15;
        }
        if(self->cooldown) --self->cooldown;
        else {
            damage(wall, 6);
			play_sound(sound_hit);
            self->cooldown = 20;
        }
        self->on_wall = 1;
    }
    else {
        if(self->on_wall) {
            set_animator(sr, &sprite_snake, 4);
        }
        parent->position = vadd(parent->position, vxy(-1.5f, 0));
        self->on_wall = 0;
    }
})

declare_head(move_snake);

entity *create_snake() {
    entity *e = ent_new();
    e->position = vxy(500, 0);
    e->tag = ENEMY;
    e->layer = 1;
    
    move_snake *mv = c_new(move_snake);
    mv->cooldown = 20;
    add_component(e, mv);
    
    add_component(e, get_enemy_health());
    add_component(e, get_animator(&sprite_snake, 4));
    add_component(e, get_health(60));
    
    return e;
}

declare_body(move_arrow_snake, {
})

declare_draw(move_arrow_snake, { })
declare_tick(move_arrow_snake, {
    sprite_renderer *sr = get_component(parent, sprite_renderer);
    if(parent->position.x < 310) {
        if(sr->animate) set_renderer(sr, &sprite_snake);
    }
    else {
        parent->position = vadd(parent->position, vxy(-1, 0));
    }
})

declare_head(move_arrow_snake);

entity *create_bow_snake() {
    entity *e = ent_new();
    e->position = vxy(500, 0);
    e->tag = ENEMY;
    e->layer = 1;
    
    move_arrow_snake *mv = c_new(move_arrow_snake);
    add_component(e, mv);
    
    add_component(e, get_enemy_health());
    add_component(e, get_animator(&sprite_snake, 4));
    add_component(e, get_animator(&sprite_spider_bow, 4));
    add_component(e, get_health(18));
    
    shoot_gun *sg = c_new(shoot_gun);
    sg->cooldown = 30;
    sg->last_shot = 20;
    sg->shoot = &create_enemy_arrow;
    add_component(e, sg);
    
    return e;
}

declare_body(move_bomb_snake, {
})

declare_draw(move_bomb_snake, { })
declare_tick(move_bomb_snake, {
    entity *wall = ent_first(within_x_range(parent->position.x, 32), within_y_range(parent->position.y, 1), tagged(TOWER), is_not(with_component(c_type(smasher_flag))));
    if(wall) {
        create_explosion(parent->position);
        for_ent(e, ent_all(
                    tagged(TOWER),
                    within_radius(parent->position, 95)
                ), {
                    float dmg = (1- (len(vsub(parent->position, e->position)) / 95)) * 34;
                    damage(e, dmg);
                })
        return 1;
    }
    else {
        parent->position = vadd(parent->position, vxy(-4, 0));
    }
})

declare_head(move_bomb_snake);

entity *create_bomb_snake() {
    entity *e = ent_new();
    e->position = vxy(500, 0);
    e->tag = ENEMY;
    e->layer = 1;
    
    add_component(e, c_new(move_bomb_snake));
    
    add_component(e, get_animator(&sprite_snake, 4));
	add_component(e, get_animator_c(&sprite_spider_bomb, 4, vxy(-13, -9)));
    enemy_health *eh = get_enemy_health();
    eh->yoff = 13;
    add_component(e, eh);
    add_component(e, get_health(28));
    
    return e;
}

declare_body(move_shield_snake, {
    int on_wall;
})

declare_draw(move_shield_snake, { })
declare_tick(move_shield_snake, {
    entity *wall = get_wall(parent, 1);
    sprite_renderer *sr = get_component(parent, sprite_renderer);
    if(wall) {
        if(!self->on_wall) {
            set_animator(sr, &sprite_snake_shield_static, 6);
        }
		self->on_wall = 1;
    }
    else {
        if(self->on_wall) {
            set_animator(sr, &sprite_snake_shield, 6);
        }
        parent->position = vadd(parent->position, vxy(-0.6f, 0));
        self->on_wall = 0;
    }
})

declare_head(move_shield_snake);

entity *create_shield_snake() {
    entity *e = ent_new();
    e->position = vxy(500, 0);
    e->tag = ENEMY;
    e->layer = 1;
    
    move_shield_snake *mv = c_new(move_shield_snake);
    add_component(e, mv);
    
    add_component(e, get_enemy_health());
    add_component(e, get_animator(&sprite_snake_shield, 6));
    add_component(e, get_health(120));
    
    return e;
}

declare_body(move_spear_snake, {
    int cooldown;
    int on_wall;
})

declare_draw(move_spear_snake, { })
declare_tick(move_spear_snake, {
    entity *wall = ent_first(within_x_range(parent->position.x, 32), within_y_range(parent->position.y, 1), tagged(TOWER));
    sprite_renderer *sr = get_component(parent, sprite_renderer);
    if(wall) {
        if(!self->on_wall) {
            set_animator(sr, &sprite_snake_spear_bash, 4);
            self->cooldown = 15;
        }
        if(self->cooldown) --self->cooldown;
        else {
            damage(wall, 12);
			play_sound(sound_hit);
            self->cooldown = 20;
        }
        self->on_wall = 1;
    }
    else {
        if(self->on_wall) {
            set_animator(sr, &sprite_snake_spear, 4);
        }
        parent->position = vadd(parent->position, vxy(-1.5f, 0));
        self->on_wall = 0;
    }
})

declare_head(move_spear_snake);

entity *create_spear_snake() {
    entity *e = ent_new();
    e->position = vxy(500, 0);
    e->tag = ENEMY;
    e->layer = 1;
    
    move_spear_snake *mv = c_new(move_spear_snake);
    mv->cooldown = 15;
    add_component(e, mv);
    
    add_component(e, get_enemy_health());
    add_component(e, get_animator(&sprite_snake_spear, 4));
    add_component(e, get_health(65));
    
    return e;
}