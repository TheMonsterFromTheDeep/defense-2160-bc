#include "event.h"
#include "entity.h"
#include "component.h"
#include "graphics.h"
#include "images_diffuse.h"
#include "tiles.h"
#include "button.h"
#include "common.h"

joystick camera;

sound *sound_build;
sound *sound_bow;
sound *sound_clicking;
sound *sound_hit;
sound *sound_cannon;
sound *sound_explosion;

mode_k mode = MENU;
mode_k switch_to = NONE;

entity *create_land(float x) {
    entity *e = ent_new();
    e->position = vxy(x, -2);
    
    add_component(e, get_renderer(&sprite_ground));
    
    return e;
}

entity *create_menu() {
	entity *e = ent_new();
	add_component(e, get_renderer(&sprite_menu));
	return e;
}

declare_body(help_handler, {
	int frame;
})

declare_draw(help_handler, { })
declare_tick(help_handler, {
	if(on_down(get_key(LEFT_MOUSE))) {
		++self->frame;
		if(self->frame > 7) switch_to = MENU;
		get_component(parent, sprite_renderer)->frame = self->frame;
	}
})
declare_head(help_handler);

entity *create_help() {
	entity *e = ent_new();
	add_component(e, get_renderer(&sprite_help));
	
	help_handler *hh = c_new(help_handler);
	hh->frame = 0;
	add_component(e, hh);
	
	return e;
}

#define PLAY_X 3
#define HELP_X 33
#define BUTTON_Y 23

mode_k main_menu = NONE;

declare_body(menu_handler, {

})

declare_draw(menu_handler, { })
declare_tick(menu_handler, {
	vec v = world_cursor();
	parent->visible = 0;
	main_menu = NONE;
	if(v.y > BUTTON_Y && v.y < BUTTON_Y + 24) {
		if(v.x > PLAY_X && v.x < PLAY_X + 24) {
			parent->position = vxy(PLAY_X, BUTTON_Y);
			parent->visible = 1;
			main_menu = GAME;
		}
		
		if(v.x > HELP_X && v.x < HELP_X + 24) {
			parent->position = vxy(HELP_X, BUTTON_Y);
			parent->visible = 1;
			main_menu = HELP;
		}
	}
})
declare_head(menu_handler);

entity *create_mh() {
	entity *e = ent_new();
	e->layer = 1;
	e->visible = 0;
	add_component(e, get_renderer(&sprite_menu_select));
	
	menu_handler *mh = c_new(menu_handler);
	add_component(e, mh);
	
	return e;
}

int wl_ticks = 0;

void init_lose() {
	camera_position(vxy(0, 0));
	
	entity *e = ent_new();
	add_component(e, get_renderer(&sprite_lose));
	
	wl_ticks = 25;
}

void init_win() {
	camera_position(vxy(0, 0));
	
	entity *e = ent_new();
	add_component(e, get_renderer(&sprite_win));
	
	wl_ticks = 25;
}

void init_menu() {
	camera_position(vxy(0, 0));
	
	create_menu();
	create_mh();
}

void init_help() {
	camera_position(vxy(0, 0));
	
	create_help();
}

void init_game() {
	init_spawns();
	init_game_values();
	
	camera_position(vxy(0, -2));
    
    set_background(rgb(0.7f, 0.9f, 0.8f));
    
	for(float x = 0; x < 500; ++x) {
        create_land(x);
    }
	
    create_core();
    create_selector();
	create_next_wave();
	
	entity *ui = ent_new();
	add_component(ui, get_renderer(&sprite_ui));
	ui->layer = 3;
	ui->fixed = 1;
	ui->position = vxy(210 - 85, 150 - 34);
	
	create_tower_selector();
	create_money_visual();
	create_price();
	
	create_spear_launcher(vxy(0, 30));
	create_spear_launcher(vxy(0, 60));
	create_spear_launcher(vxy(0, 90));
	create_spear_launcher(vxy(0, 120));
	
	total_money = 700;
}

void init() {
	sound_init();
	set_music("snd/music.wav");
	
	sound_build = get_sound("snd/build.wav");
	sound_bow = get_sound("snd/bow.wav");
	sound_clicking = get_sound("snd/clicking.wav");
	sound_hit = get_sound("snd/hit.wav");
	sound_cannon = get_sound("snd/cannon.wav");
	sound_explosion = get_sound("snd/explosion.wav");
	
	camera = joystick_from(key_axis('A', 'D'), key_axis('S', 'W'));
	
	init_menu();
}

void set_mode(mode_k which) {
	ent_cleanup();
	mode = which;
	
	switch(mode) {
		case MENU:
			init_menu();
			break;
		case GAME:
			init_game();
			break;
		case HELP:
			init_help();
			break;
		case LOSE:
			init_lose();
			break;
		case WIN:
			init_win();
			break;
		default: { }
	}
}

void loop() {
    ent_process();

	switch(mode) {
		case GAME:
			camera_shift(vmul(joystick_value(camera), 10));
			clamp_camera(0, 500 - 210, -2, 20);
		
			enemy_spawn();
			
			if(!ent_first(with_component(c_type(core)))) switch_to = LOSE;
			break;
			
		case MENU: 
			if(on_down(get_key(LEFT_MOUSE))) {
				if(main_menu != NONE) {
					set_mode(main_menu);
				}
			}
			break;
			
		case WIN:
		case LOSE:
			if(wl_ticks) --wl_ticks;
			else if(on_down(get_key(LEFT_MOUSE))) {
				set_mode(MENU);
			}
			break;
			
		default: { }
	}
	
	if(switch_to != NONE) {
		set_mode(switch_to);
		switch_to = NONE;
	}
}