#include "common.h"

#include <stdlib.h>
#include <math.h>

int total_money = 700;

zlist_of(enemy_spawner) spider_types = NULL;
zlist_of(enemy_spawner) snake_types = NULL;
zlist_of(enemy_spawner) bat_types = NULL;

declare_draw(hold_upgrade, { })
declare_tick(hold_upgrade, { })

declare_head(hold_upgrade);

hold_upgrade *upgrader(enemy_spawner spider, enemy_spawner snake, enemy_spawner bat) {
    hold_upgrade *hu = c_new(hold_upgrade);
    hu->spider = spider;
	hu->snake = snake;
	hu->bat = bat;
    return hu;
}

static int current_tower_selected;

declare_body(tower_selector, {})

declare_draw(tower_selector, { })
declare_tick(tower_selector, {
	float y = get_scroll().y;
	if(y > 0) {
		++current_tower_selected;
	}
	if(y < 0) {
		--current_tower_selected;
	}
	if(current_tower_selected < 0) current_tower_selected = (TOWER_COUNT - 1);
	if(current_tower_selected > (TOWER_COUNT - 1)) current_tower_selected = 0;
	
	sprite_renderer *sr = get_component(parent, sprite_renderer);
	sr->frame = current_tower_selected;
})

declare_head(tower_selector)

entity *create_tower_selector() {
	entity *e = ent_new();
	e->layer = 3;
	e->fixed = 1;
	e->position = vxy(210 - 32, 150 - 32);
	
	add_component(e, get_renderer(&sprite_towers));
	
	tower_selector *ts = c_new(tower_selector);
	current_tower_selected = 0;
	add_component(e, ts);
	
	return e;
}

declare_body(update_price, { })
declare_draw(update_price, { })
declare_tick(update_price, { get_component(parent, number_renderer)->value = &prices[current_tower_selected]; })
declare_head(update_price);

entity *create_price() {
	entity *e = ent_new();
	e->layer = 3;
	e->fixed = 1;
	e->position = vxy(210 - 43, 150 - 32);
	
	number_renderer *nr = c_new(number_renderer);
	nr->value = &prices[0];
	nr->xoff = 0; nr->yoff = 0;
	add_component(e, nr);
	
	add_component(e, c_new(update_price));
	
	return e;
}

entity *create_money_visual() {
	entity *e = ent_new();
	e->layer = 3;
	e->fixed = 1;
	e->position = vxy(210 - 43, 150 - 13);
	
	number_renderer *nr = c_new(number_renderer);
	nr->value = &total_money;
	nr->xoff = 0; nr->yoff = 0;
	add_component(e, nr);
	
	return e;
}

void init_spawns() {
	if(spider_types) zfree(spider_types);
    zlist_init(spider_types, 1);
    spider_types[0] = &create_spider;
	
	if(snake_types) zfree(snake_types);
	zlist_init(snake_types, 1);
    snake_types[0] = &create_snake;
	
	if(snake_types) zfree(bat_types);
	zlist_init(bat_types, 1);
	bat_types[0] = &create_bat;
}

void spawn_spider() {
	int spider_type = rand() % zsize(spider_types);
    spider_types[spider_type]();
}

void spawn_snake() {
	int snake_type = rand() % zsize(snake_types);
    snake_types[snake_type]();
}

void spawn_bat() {
	int bat_type = rand() % zsize(bat_types);
    bat_types[bat_type]();
}

int spider_quota = 0, snake_quota = 0, bat_quota = 0;
int spider_delay = 0, snake_delay = 0, bat_delay = 0;
int spider_ticks = 0, snake_ticks = 0, bat_ticks = 0;
int which_wave = 0;

static int next_wave_ticks = 50 * 9;
static int should_give_money = 0;

void init_game_values() {
	total_money = 700;
	current_tower_selected = 0;

	spider_quota = 0, snake_quota = 0, bat_quota = 0;
	spider_delay = 0, snake_delay = 0, bat_delay = 0;
	spider_ticks = 0, snake_ticks = 0, bat_ticks = 0;
	which_wave = 0;

	next_wave_ticks = 50 * 9;
	should_give_money = 0;
}

int get_ticks(int delay) {
	return delay - 5 + (rand() % 11);
}

void populate_ticks() {
	spider_ticks = get_ticks(spider_delay);
	snake_ticks  = get_ticks(snake_delay);
	bat_ticks    = get_ticks(bat_delay);
}

int process(int *ticks, int *delay, int *quota) {
	if(*ticks) --*ticks;
	else {
		*ticks = *delay;
		if(*quota) {
			--*quota;
			return 1;
		}
		else return 0;
	}
	return 0;
}

void enemy_spawn() {
    /*if(spider_ticks) --spider_ticks;
    else {
        
        spider_ticks = 5000 + (rand() % 50);
        //create_shield_snake();
		//create_shield_bat();
		
		//spawn_bat();
		create_spear_snake();
    }*/
	if(spider_quota || snake_quota || bat_quota) {
		if(process(&spider_ticks, &spider_delay, &spider_quota)) spawn_spider();
		if(process(&snake_ticks,  &snake_delay,  &snake_quota )) spawn_snake ();
		if(process(&bat_ticks,    &bat_delay,    &bat_quota   )) spawn_bat   ();
	}
	else if(next_wave_ticks) {
		--next_wave_ticks;
		if(should_give_money) {
			total_money += 100 + which_wave * 10;
			should_give_money = 0;
		}
		if(which_wave >= WAVE_COUNT) {
			switch_to = WIN;
		}
	}
	else {
		spider_ticks = 0;
		snake_ticks  = 0;
		bat_ticks    = 0;
		
		spider_quota = waves[which_wave].spider_quota;
		snake_quota  = waves[which_wave].snake_quota;
		bat_quota    = waves[which_wave].bat_quota;
		
		spider_delay = waves[which_wave].spider_rate;
		snake_delay  = waves[which_wave].snake_rate;
		bat_delay    = waves[which_wave].bat_rate;
		
		populate_ticks();
		
		++which_wave;
		next_wave_ticks = (50 * 9) - which_wave;
		
		should_give_money = 1;
	}
}

static int next_wave_sec;

declare_body(next_wave_hide, { })

declare_draw(next_wave_hide, { })
declare_tick(next_wave_hide, {
	next_wave_sec = next_wave_ticks / 50;
	parent->visible = !(spider_quota || snake_quota || bat_quota);
})

declare_head(next_wave_hide);

entity *create_next_wave() {
	entity *e = ent_new();
	e->position = vxy(210 - 15, 0);
	e->layer = 3;
	e->fixed = 1;
	
	add_component(e, get_renderer(&sprite_next_wave));
	add_component(e, c_new(next_wave_hide));
	
	number_renderer *nr = c_new(number_renderer);
	nr->xoff = 3; nr->yoff = 3;
	nr->value = &next_wave_sec;
	
	add_component(e, nr);
	
	return e;
}

void block_place_particles(vec pos, float sign) {
    entity *e = ent_new();
    e->position = pos;
    e->layer = 2;
    
    add_component(e, create_particle_system(
        1,
        6,
        20,
        vxy(sign, 1.f),
        vy(-0.9f),
        0.2f,
        &sprite_block_particle
    ));
    
    add_component(e, get_particle_killer(21));
}

declare_body(selector_control, {
    
})

declare_draw(selector_control, { })
declare_tick(selector_control, {
    vec mouse = world_cursor();
    mouse = vsub(mouse, vxy(15, 15));
    mouse = vsnap(mouse, 30);
    if(mouse.y < 0) mouse.y = 0;
	if(mouse.y > 120) mouse.y = 120;
    if(mouse.x < 0) mouse.x = 0;
    if(mouse.x > 300) mouse.x = 300;
    parent->position = mouse;
    
    sprite_renderer *sr = get_component(parent, sprite_renderer);
    
    entity *tower = ent_first(tagged(TOWER), within_radius(parent->position, 1));
    int valid = 0;
    sr->spr = &sprite_selector_cannot_place;
    
    if(tower) {
		sr->spr = full_refund(tower) ? &sprite_selector_refund : &sprite_selector_select;
	}
    else {
        entity *top    = ent_first(tagged(TOWER), within_radius(vadd(parent->position, vy(30)), 1));
        entity *bottom = ent_first(tagged(TOWER), within_radius(vsub(parent->position, vy(30)), 1));
        entity *right  = ent_first(tagged(TOWER), within_radius(vadd(parent->position, vx(30)), 1));
        entity *left   = ent_first(tagged(TOWER), within_radius(vsub(parent->position, vx(30)), 1));
        if(top || bottom || right || left) {
			if(prices[current_tower_selected] <= total_money) {
				sr->spr = &sprite_selector_valid;
				valid = 1;
			} else { sr->spr = &sprite_selector_invalid; }
        }
    }
    
    if(on_down(get_key(LEFT_MOUSE))) {
        if(tower) {
            if(!get_component(tower, core)) {
				refund(tower);
				ent_destroy(tower);
			}
			play_sound(sound_build);
        }
        else if(valid) {
            towers[current_tower_selected](mouse);
			total_money -= prices[current_tower_selected];
			play_sound(sound_build);
        }
        else { goto no_particles; }
        
        block_place_particles(parent->position, -1);
        block_place_particles(vadd(parent->position, vx(30)), 1);
        
        no_particles: { }
    }
})

declare_head(selector_control);

entity *create_selector() {
    entity *e = ent_new();
    e->layer = 3;
    
    add_component(e, get_renderer(&sprite_selector_invalid));
    
    selector_control *cs = c_new(selector_control);
    add_component(e, cs);
    
    return e;
}