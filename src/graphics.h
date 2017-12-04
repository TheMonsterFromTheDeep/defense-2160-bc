#ifndef RD_GRAPHICS_H
#define RD_GRAPHICS_H

#include "component.h"
#include "vector.h"

typedef struct {
    float r, g, b;
} color;

typedef struct {
    float x0, y0, x1, y1;
    float width, height;
    int frames;
    float frame_height;
} sprite;

declare_body(sprite_renderer, {
    sprite *spr;
    vec center;
    int frame;
    int delay;
    int cooldown;
    int animate;
})

sprite_renderer *get_renderer(sprite*);
sprite_renderer *get_animator(sprite*,int);
sprite_renderer *get_animator_c(sprite*,int,vec);

void set_renderer(sprite_renderer*,sprite*);
void set_animator(sprite_renderer*,sprite*,int);
void set_animator_c(sprite_renderer*,sprite*,int,vec);

color rgb(float,float,float);
color xrgb(int);

void camera_position(vec);
void camera_shift(vec);

void set_background(color);
void draw_sprite(const sprite*,float,float);
void draw_sprite_f(const sprite*,float,float,int);
void draw_sprite_a(const sprite*,float,float,float);
void draw_sprite_fa(const sprite*,float,float,float,int);
void draw_rect(color,float,float,float,float);

int get_cam_width();
int get_cam_height();

void take_screenshot(int);

void clamp_camera(float,float,float,float);

#endif