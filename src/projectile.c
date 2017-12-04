#include "common.h"

declare_body(move_projectile, {
    vec velocity;
    int lifetime;
    int piercing;
    int tag;
    float damage;
    float splash;
})

declare_draw(move_projectile, { })
declare_tick(move_projectile, {
    parent->position = vadd(parent->position, self->velocity);
    
    entity *enemy = ent_first(tagged(self->tag), within_radius(parent->position, 7.5f));
    if(enemy) {
        damage(enemy, self->damage);
        
        if(self->piercing) --self->piercing;
        else {
            if(self->splash) {
                for_ent(e, ent_all(
                    tagged(self->tag),
                    within_radius(parent->position, self->splash),
                    not_entity(enemy)
                ), {
                    float dmg = (1- (len(vsub(parent->position, e->position)) / self->splash)) * self->damage;
                    damage(e, dmg);
                })
                
                create_explosion(parent->position);
            }
            return 1;
        }
    }
    
    --self->lifetime;
    return !self->lifetime;
})

declare_head(move_projectile);

entity *create_cannonball(vec pos) {
	play_sound(sound_cannon);
    entity *e = ent_new();
    e->position = vadd(pos, vxy(25, 2));
    e->layer = 1;
    
    add_component(e, get_renderer(&sprite_cannonball));
    
    move_projectile *mv = c_new(move_projectile);
    mv->velocity = vxy(7.f, 0.f);
    mv->lifetime = 500;
    mv->piercing = 0;
    mv->damage = 50;
    mv->splash = 80;
    mv->tag = ENEMY;
    add_component(e, mv);
    
    return e;
}

entity *create_arrow(vec pos) {
	play_sound(sound_bow);
	
    entity *e = ent_new();
    e->position = vadd(pos, vxy(0, 5));
    e->layer = 1;
    
    add_component(e, get_renderer(&sprite_arrow));
    
    move_projectile *mv = c_new(move_projectile);
    mv->velocity = vxy(9.f, 0.f);
    mv->lifetime = 300;
    mv->piercing = 3;
    mv->damage = 4;
    mv->splash = 0;
    mv->tag = ENEMY;
    add_component(e, mv);
    
    return e;
}

entity *create_enemy_arrow(vec pos) {
	play_sound(sound_bow);
	
    entity *e = ent_new();
    e->position = vadd(pos, vxy(5, 4));
    
    add_component(e, get_renderer(&sprite_enemy_arrow));
    
    move_projectile *mv = c_new(move_projectile);
    mv->velocity = vxy(-7.f, 0.f);
    mv->lifetime = 300;
    mv->piercing = 1;
    mv->damage = 2;
    mv->splash = 0;
    mv->tag = TOWER;
    add_component(e, mv);
    
    return e;
}