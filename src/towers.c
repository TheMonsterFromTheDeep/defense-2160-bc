#include "common.h"

declare_draw(core, { })
declare_tick(core, { })
declare_head(core);

entity *create_core() {
    entity *e = ent_new();
    e->position = v0();
    e->tag = TOWER;
    
    add_component(e, c_new(core));
    
    add_component(e, get_renderer(&sprite_core));
    
    add_component(e, get_health(200));
    add_component(e, c_new(tower_health));
    add_component(e, get_healer(0.05f));
    
    return e;
}

declare_body(tower_grace, { int ticks; int cost; })
declare_draw(tower_grace, { })
declare_tick(tower_grace, {
	if(self->ticks) {
		--self->ticks;
		if(!self->ticks) self->cost /= 10;
	}
})
declare_head(tower_grace);
tower_grace *get_grace(int cost) { tower_grace* tg = c_new(tower_grace); tg->ticks = 50; tg->cost = cost; return tg; }

#define SHIELD_PRICE 50
#define BOW_PRICE 100
#define BOMB_PRICE 200
#define SMASH_PRICE 300
#define SPEAR_PRICE 150

int prices[TOWER_COUNT] = {
	SHIELD_PRICE,
	BOW_PRICE,
	BOMB_PRICE,
	SMASH_PRICE,
	SPEAR_PRICE
};

create_tower towers[TOWER_COUNT] = {
	&create_shield,
	&create_arrow_launcher,
	&create_gun,
	&create_smasher,
	&create_spear_launcher
};

void refund(entity *e) {
	tower_grace *tg = get_component(e, tower_grace);
	if(tg) total_money += tg->cost;
}

int full_refund(entity *e) {
	tower_grace *tg = get_component(e, tower_grace);
	if(tg) {
		return tg->ticks;
	}
}

entity *create_shield(vec pos) {
    entity *e = ent_new();
    e->position = pos;
    e->tag = TOWER;
    
    add_component(e, get_animator(&sprite_shield, 4));
    
    add_component(e, get_health(250));
    add_component(e, c_new(tower_health));
    add_component(e, get_healer(0.03f));
    add_component(e, upgrader(&create_shield_spider, &create_shield_snake, &create_shield_bat));
	
	add_component(e, get_grace(SHIELD_PRICE));
    
    return e;
}

entity *create_gun(vec pos) {
    entity *e = ent_new();
    e->position = pos;
    e->tag = TOWER;
    
    add_component(e, get_animator(&sprite_gun, 4));
    
    shoot_gun *sg = c_new(shoot_gun);
    sg->cooldown = 65;
    sg->last_shot = 25;
    sg->shoot = &create_cannonball;
    add_component(e, sg);
    
    add_component(e, get_health(30));
    add_component(e, c_new(tower_health));
    add_component(e, get_healer(0.01f));
    add_component(e, upgrader(&create_bomb_spider, &create_bomb_snake, &create_bomb_bat));
	
	add_component(e, get_grace(BOMB_PRICE));
    
    return e;
}

entity *create_arrow_launcher(vec pos) {
    entity *e = ent_new();
    e->position = pos;
    e->tag = TOWER;
    
    add_component(e, get_animator(&sprite_arrow_launcher, 4));
    
    shoot_gun *sg = c_new(shoot_gun);
    sg->cooldown = 40;
    sg->last_shot = 20;
    sg->shoot = &create_arrow;
    add_component(e, sg);
    
    add_component(e, get_health(60));
    add_component(e, c_new(tower_health));
    add_component(e, upgrader(&create_bow_spider, &create_bow_snake, &create_bow_bat));
    add_component(e, get_healer(0.02f));
    
	add_component(e, get_grace(BOW_PRICE));
	
    return e;
}

declare_body(smasher, {
    entity *top;
})

declare_draw(smasher, { })
declare_tick(smasher, { ent_destroy(self->top); })

declare_head(smasher);

declare_draw(smasher_flag, { })
declare_tick(smasher_flag, {
    float dmg = 0;
    switch(self->tick) {
        case 2:
        dmg = 2;
        break;
        case 3:
        dmg = 5;
    }
    
    float x = parent->position.x, y = parent->position.y;
    
    for_ent(e, ent_all(
            within_x_bounds(x, x + 30),
            within_y_bounds(y, y + 10),
            tagged(ENEMY)
        ), {
            damage(e, dmg);
    });
    
    ++self->tick;
    if(self->tick >= 7) self->tick = 0;
})
declare_head(smasher_flag);

entity *create_smasher_top(vec pos) {
    entity *e = ent_new();
    e->position = pos;
    e->layer = 2;
    
    add_component(e, get_animator(&sprite_smasher_top, 4));
    
    return e;
}

entity *create_smasher(vec pos) {
    entity *e = ent_new();
    e->position = pos;
    e->tag = TOWER;
    
    add_component(e, get_animator(&sprite_smasher, 4));
    
    add_component(e, get_health(60));
    add_component(e, c_new(tower_health));
    add_component(e, get_healer(0.008f));
    
    
    smasher_flag *sf = c_new(smasher_flag);
    sf->tick = 0;
    add_component(e, sf);
    //add_component(e, upgrader(&create_bomb_spider));
    
    smasher *s = c_new(smasher);
    s->top = create_smasher_top(pos);
    e->kill_handler = s;
	
	add_component(e, get_grace(SMASH_PRICE));
    
    return e;
}

declare_body(spear_launcher, {
	int cooldown;
})

declare_draw(spear_launcher, { })
declare_tick(spear_launcher, {
	if(self->cooldown) --self->cooldown;
	else {
		for_ent(e,
		ent_all(
			within_x_bounds(parent->position.x, parent->position.x + 20),
			within_y_bounds(parent->position.y + 30, parent->position.y + 40),
			tagged(ENEMY)
		),
		{
			damage(e, 13);
		})
		self->cooldown = 25;
	}
})

declare_head(spear_launcher)

entity *create_spear_launcher(vec pos) {
    entity *e = ent_new();
    e->position = pos;
    e->tag = TOWER;
    
    
    
    add_component(e, get_health(120));
    add_component(e, c_new(tower_health));
    //add_component(e, upgrader(&create_bow_spider, &create_bow_snake, &create_bow_bat));
    add_component(e, get_healer(0.01f));
	
	spear_launcher *sl = c_new(spear_launcher);
	sl->cooldown = 20;
	add_component(e, sl);
    
	add_component(e, get_animator(&sprite_spear_launcher, 4));
	add_component(e, get_grace(SPEAR_PRICE));
	
    return e;
}