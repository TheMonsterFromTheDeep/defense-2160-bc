#include "graphics.h"
#include "entity.h"
#include "button.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

#include "images_diffuse.h"

declare_draw(sprite_renderer, {
    if(!self->spr) { return 0; }
    vec position = vmul(self->center, -1);
    if(parent) {
        position = vadd(position, parent->position);
    }
    draw_sprite_f(self->spr, position.x, position.y, self->frame);
})

declare_tick(sprite_renderer, {
    if(self->animate) {
        if(self->cooldown) {
            --self->cooldown;
        }
        else {
            self->frame = (self->frame + 1) % self->spr->frames;
            self->cooldown = self->delay;
        }
    }
})

declare_head(sprite_renderer)

color background;

static int camera_width, camera_height;

int get_cam_width() { return camera_width; }
int get_cam_height() { return camera_height; }

sprite_renderer *get_renderer(sprite *s) {
    sprite_renderer *sr = c_new(sprite_renderer);
    sr->center = v0();
    sr->spr = s;
    sr->frame = 0;
    sr->cooldown = 0;
    sr->animate = 0;
    return sr;
}

sprite_renderer *get_animator(sprite *s, int delay) {
    sprite_renderer *sr = c_new(sprite_renderer);
    sr->center = v0();
    sr->spr = s;
    sr->frame = 0;
    sr->delay = delay;
    sr->cooldown = delay;
    sr->animate = 1;
    return sr;
}

sprite_renderer *get_animator_c(sprite *s, int delay, vec center) {
    sprite_renderer *sr = c_new(sprite_renderer);
    sr->center = center;
    sr->spr = s;
    sr->frame = 0;
    sr->delay = delay;
    sr->cooldown = delay;
    sr->animate = 1;
    return sr;
}

void set_renderer(sprite_renderer* sr, sprite *s) {
    sr->center = v0();
    sr->spr = s;
    sr->frame = 0;
    sr->cooldown = 0;
    sr->animate = 0;
}

void set_animator(sprite_renderer* sr, sprite *s, int delay) {
    sr->center = v0();
    sr->spr = s;
    sr->frame = 0;
    sr->delay = delay;
    sr->cooldown = delay;
    sr->animate = 1;
}

void set_animator_c(sprite_renderer* sr, sprite *s, int delay, vec center) {
    sr->center = center;
    sr->spr = s;
    sr->frame = 0;
    sr->delay = delay;
    sr->cooldown = delay;
    sr->animate = 1;
}

static GLuint spritesheet;

GLuint load_image_diffuse();
GLuint load_image_normal();
GLuint load_image_depth();

void load_textures() {
    spritesheet = load_image_diffuse();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void set_camera_size(int width, int height) {
    camera_width = width;
    camera_height = height;
}

color rgb(float r, float g, float b) {
    return (color){ r, g, b };
}

#define ONE_OVER_255 0.00392156862

color xrgb(int hex) {
    int red =   0xff & (hex >> 16);
    int green = 0xff & (hex >>  8);
    int blue =  0xff & (hex      );
    return rgb(red * ONE_OVER_255, green * ONE_OVER_255, blue * ONE_OVER_255);
}

void set_background(color bg) {
    background = bg;
}

vec camera_position_val;

void camera_position(vec v) {
    camera_position_val = v;
}

void camera_shift(vec v) {
    camera_position_val = vadd(camera_position_val, v);
}

void clamp_camera(float xi, float xa, float yi, float ya) {
    if(camera_position_val.x < xi) { camera_position_val.x = xi; }
    if(camera_position_val.x > xa) { camera_position_val.x = xa; }
    if(camera_position_val.y < yi) { camera_position_val.y = yi; }
    if(camera_position_val.y > ya) { camera_position_val.y = ya; }
}

vec world_cursor() {
    return vadd(screen_cursor(), camera_position_val);
}

void render() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(background.r, background.g, background.b);
    
    glBegin(GL_QUADS);
        glVertex2f(0.f, 0.f);
        glVertex2f(0.f, camera_height);
        glVertex2f(camera_width, camera_height);
        glVertex2f(camera_width, 0);
    glEnd();
    
    glTranslatef(-camera_position_val.x, -camera_position_val.y, 0);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    glBindTexture(GL_TEXTURE_2D, spritesheet);
    glColor3f(1.f, 1.f, 1.f);
    
    //draw_sprite(&sprite_core, 0, 0);
    //draw_sprite(&sprite_gun, 0, 30);
    
    size_t count = 0;
    
    for_ent(e, ent_all(any()), {
		if(e->visible) {
			if(e->fixed) {
				glPushMatrix();
				glLoadIdentity();
			}
			//printf("drawing %d entity\n", count++);
			for(size_t i = 0; i < zsize(e->components); ++i) {
				//printf("drawing %d component\n", i);
				c_draw(e->components[i]);
			}
			
			if(e->fixed) {
				glPopMatrix();
			}
		}
    })
}

void draw_rect(color c, float x, float y, float width, float height) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(c.r, c.g, c.b);
    
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        
        y += height;
        
        glVertex2f(x, y);
        
        x += width;
        
        glVertex2f(x, y);
        
        y -= height;
        
        glVertex2f(x, y);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, spritesheet);
    glColor3f(1.f, 1.f, 1.f);
}

void draw_sprite_fa(const sprite *s, float x, float y, float a, int frame) { 
    glColor4f(1.f, 1.f, 1.f, a);
   
    if(frame < 0) frame = 0;
    if(frame >= s->frames) frame = s->frames - 1;
    float yoff = frame * s->frame_height;
   
    glBegin(GL_QUADS);
        glTexCoord2f(s->x0, s->y1 + yoff);
        glVertex2f(x, y);
        
        y += s->height;
        
        glTexCoord2f(s->x0, s->y0 + yoff);
        glVertex2f(x, y);
        
        x += s->width;
        
        glTexCoord2f(s->x1, s->y0 + yoff);
        glVertex2f(x, y);
        
        y -= s->height;
        
        glTexCoord2f(s->x1, s->y1 + yoff);
        glVertex2f(x, y);
    glEnd();
}

void draw_sprite(const sprite *s, float x, float y) { 
   draw_sprite_a(s, x, y, 1);
}

void draw_sprite_a(const sprite *s, float x, float y, float a) {
    draw_sprite_fa(s, x, y, a, 0);
}

void draw_sprite_f(const sprite *s, float x, float y, int frame) {
    draw_sprite_fa(s, x, y, 1, frame);
}