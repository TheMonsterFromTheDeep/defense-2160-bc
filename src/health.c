#include "common.h"

declare_draw(health, { })
declare_tick(health, {
    self->visual += (self->amount - self->visual) * 0.2f;
    if(self->amount <= 0) {
        hold_upgrade *hu = get_component(parent, hold_upgrade);
        if(hu) {
            for(size_t i = 0; i < zsize(spider_types); ++i) {
                if(spider_types[i] == hu->spider) {
                    goto already_have_it;
                }
            }
            
            zlist_add(spider_types, hu->spider);
			zlist_add(snake_types, hu->snake);
			zlist_add(bat_types, hu->bat);
            
            already_have_it: { }
        }
		
		if(parent->tag == ENEMY) {
			int base = (int)(self->total / 10);
			if(base < 2) base = 2;
			total_money += base + rand() % 5;
		}
    }
    return self->amount <= 0;
})

declare_head(health);

void damage(entity* e, float amount) {
    health *h = get_component(e, health);
    if(h) {
        h->amount -= amount;
        if(h->amount > h->total) h->amount = h->total;
    }
}

health *get_health(float total) {
    health *h = c_new(health);
    h->amount = total;
    h->visual = total;
    h->total = total;
    return h;
}

declare_draw(tower_health, {
    health *h = get_component(parent, health);
    float width = 28 * (h->visual / h->total);
	draw_rect(xrgb(0xbe945a), parent->position.x + 1, parent->position.y + 27, 28, 2);
    draw_rect(xrgb(0xe1a952), parent->position.x + 1, parent->position.y + 27, width, 2);
})
declare_tick(tower_health, { })

declare_head(tower_health);

declare_draw(enemy_health, {
    sprite_renderer *sr = get_component(parent, sprite_renderer);
    float W = sr->spr->width, H = sr->spr->height;
    health *h = get_component(parent, health);
    float width = W * (h->visual / h->total);
    draw_rect(xrgb(0xff0303), parent->position.x + self->xoff, parent->position.y + H + 1 + self->yoff, width, 2);
})
declare_tick(enemy_health, { })

declare_head(enemy_health);

enemy_health *get_enemy_health() {
    enemy_health *eh = c_new(enemy_health);
    eh->xoff = 0;
    eh->yoff = 0;
    return eh;
}

declare_draw(healer, { })
declare_tick(healer, {
    damage(parent, -self->rate);
})

declare_head(healer);

healer *get_healer(float rate) {
    healer *h = c_new(healer);
    h->rate = rate;
    return h;
}
