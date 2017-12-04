#include "common.h"

declare_body(move_spider, {
    int cooldown;
    int on_wall;
})

declare_draw(move_spider, { })
declare_tick(move_spider, {
    entity *wall = ent_first(within_x_range(parent->position.x, 32), within_y_range(parent->position.y, 1), tagged(TOWER), is_not(with_component(c_type(smasher_flag))));
    sprite_renderer *sr = get_component(parent, sprite_renderer);
    if(wall) {
        if(!self->on_wall) {
            set_animator(sr, &sprite_spider_bash, 4);
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
            set_animator(sr, &sprite_spider, 4);
        }
        parent->position = vadd(parent->position, vxy(-1, 0));
        self->on_wall = 0;
    }
})

declare_head(move_spider);

entity *create_spider() {
    entity *e = ent_new();
    e->position = vxy(500, 0);
    e->tag = ENEMY;
    e->layer = 1;
    
    move_spider *mv = c_new(move_spider);
    mv->cooldown = 20;
    add_component(e, mv);
    
    add_component(e, get_enemy_health());
    add_component(e, get_animator(&sprite_spider, 4));
    add_component(e, get_health(40));
    
    return e;
}

declare_body(move_arrow_spider, {
})

declare_draw(move_arrow_spider, { })
declare_tick(move_arrow_spider, {
    sprite_renderer *sr = get_component(parent, sprite_renderer);
    if(parent->position.x < 310) {
        if(sr->animate) set_renderer(sr, &sprite_spider);
    }
    else {
        parent->position = vadd(parent->position, vxy(-1, 0));
    }
})

declare_head(move_arrow_spider);

entity *create_bow_spider() {
    entity *e = ent_new();
    e->position = vxy(500, 0);
    e->tag = ENEMY;
    e->layer = 1;
    
    move_arrow_spider *mv = c_new(move_arrow_spider);
    add_component(e, mv);
    
    add_component(e, get_enemy_health());
    add_component(e, get_animator(&sprite_spider, 4));
    add_component(e, get_animator(&sprite_spider_bow, 4));
    add_component(e, get_health(13));
    
    shoot_gun *sg = c_new(shoot_gun);
    sg->cooldown = 30;
    sg->last_shot = 20;
    sg->shoot = &create_enemy_arrow;
    add_component(e, sg);
    
    return e;
}

declare_body(move_bomb_spider, {
})

declare_draw(move_bomb_spider, { })
declare_tick(move_bomb_spider, {
    entity *wall = ent_first(within_x_range(parent->position.x, 32), within_y_range(parent->position.y, 1), tagged(TOWER), is_not(with_component(c_type(smasher_flag))));
    if(wall) {
        create_explosion(parent->position);
        for_ent(e, ent_all(
                    tagged(TOWER),
                    within_radius(parent->position, 95)
                ), {
                    float dmg = (1- (len(vsub(parent->position, e->position)) / 95)) * 30;
                    damage(e, dmg);
                })
        return 1;
    }
    else {
        parent->position = vadd(parent->position, vxy(-3, 0));
    }
})

declare_head(move_bomb_spider);

entity *create_bomb_spider() {
    entity *e = ent_new();
    e->position = vxy(500, 0);
    e->tag = ENEMY;
    e->layer = 1;
    
    add_component(e, c_new(move_bomb_spider));
    
    add_component(e, get_animator_c(&sprite_spider_bomb, 4, vxy(-3, -11)));
    add_component(e, get_animator(&sprite_spider, 4));
    enemy_health *eh = get_enemy_health();
    eh->xoff = 2;
    eh->yoff = 13;
    add_component(e, eh);
    add_component(e, get_health(25));
    
    return e;
}

declare_body(move_shield_spider, {
    int on_wall;
})

declare_draw(move_shield_spider, { })
declare_tick(move_shield_spider, {
    entity *wall = get_wall(parent, 1);
    sprite_renderer *sr = get_component(parent, sprite_renderer);
    if(wall) {
        if(!self->on_wall) {
            set_animator(sr, &sprite_spider_shield_static, 8);
        }
		self->on_wall = 1;
    }
    else {
        if(self->on_wall) {
            set_animator(sr, &sprite_spider_shield, 8);
        }
        parent->position = vadd(parent->position, vxy(-0.3f, 0));
        self->on_wall = 0;
    }
})

declare_head(move_shield_spider);

entity *create_shield_spider() {
    entity *e = ent_new();
    e->position = vxy(500, 0);
    e->tag = ENEMY;
    e->layer = 1;
    
    move_shield_spider *mv = c_new(move_shield_spider);
    add_component(e, mv);
    
    add_component(e, get_enemy_health());
    add_component(e, get_animator(&sprite_spider_shield, 8));
    add_component(e, get_health(80));
    
    return e;
}

declare_body(move_spear_spider, {
    int cooldown;
    int on_wall;
})

declare_draw(move_spear_spider, { })
declare_tick(move_spear_spider, {
    entity *wall = ent_first(within_x_range(parent->position.x, 32), within_y_range(parent->position.y, 1), tagged(TOWER), is_not(with_component(c_type(smasher_flag))));
    sprite_renderer *sr = get_component(parent, sprite_renderer);
    if(wall) {
        if(!self->on_wall) {
            set_animator_c(sr, &sprite_spider_spear_bash, 4, vxy(4, 0));
            self->cooldown = 15;
        }
        if(self->cooldown) --self->cooldown;
        else {
            damage(wall, 9);
			play_sound(sound_hit);
            self->cooldown = 20;
        }
        self->on_wall = 1;
    }
    else {
        if(self->on_wall) {
            set_animator(sr, &sprite_spider_spear, 4);
        }
        parent->position = vadd(parent->position, vxy(-1.2f, 0));
        self->on_wall = 0;
    }
})

declare_head(move_spear_spider);

entity *create_spear_spider() {
    entity *e = ent_new();
    e->position = vxy(500, 0);
    e->tag = ENEMY;
    e->layer = 1;
    
    move_spear_spider *mv = c_new(move_spear_spider);
    mv->cooldown = 20;
    add_component(e, mv);
    
    add_component(e, get_enemy_health());
    add_component(e, get_animator(&sprite_spider_spear, 4));
    add_component(e, get_health(45));
    
    return e;
}