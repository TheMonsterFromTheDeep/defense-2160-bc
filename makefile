# Ccompiler
CC= gcc

# Executable name
BIN= rd-test

# Source and build paths
SRC_DIR= ./src
BUILD_DIR= ./build

# Source files
SRCS= main.c component.c window.c button.c loops.c event.c renderer.c glew.c images_diffuse.c vector.c entity.c rdtest.c tiles.c screenshot.c particle.c towers.c projectile.c spider.c snake.c bat.c health.c tools.c wave.c icon.c

# Sprites
SPRS=core.png gun.png ground.png cannonball.png spider.png selector_valid.png selector_invalid.png selector_select.png arrow_launcher.png arrow.png arrow_spider.png enemy_arrow.png flame_particle.png smoke_particle.png block_particle.png spider_bash.png spider_bow.png spider_bomb.png smasher.png smasher_top.png snake.png towers.png ui.png snake_bash.png number.png selector_cannot_place.png bat.png selector_refund.png bat_side_bash.png bat_low_bash.png shield.png spider_shield.png snake_shield.png spider_shield_static.png snake_shield_static.png bat_shield.png spear_launcher.png spider_spear.png spider_spear_bash.png bat_spear_bash.png bat_spear.png snake_spear.png snake_spear_bash.png next_wave.png menu.png help.png

# Object files to build
OBJS= $(SRCS:%.c=%.o)

# Dependencies for each source
DEPS= $(OBJS:%.o=$(BUILD_DIR)/%.d)

# Flags for the compiler
CFLAGS= -Wall -Wno-unused-variable -lglfw3 -lopengl32 -lgdi32 -lpthread -lzgcl -lsdl2 -lsdl2_mixer

# Default path for make install
INSTALL_PATH?=/usr/local

$(BIN) : $(OBJS:%=$(BUILD_DIR)/%)
	$(CC) $^ build/shoehorn_sdl.o -o $@ $(CFLAGS)

-include $(DEPS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) -MMD -c $< -o $@ $(CFLAGS)
	
shoehorn:
	gcc -c src/shoehorn_sdl.c -o build/shoehorn_sdl.o -lmingw32 -lsdl2 -lsdl2_mixer -IC:\MinGW\include\SDL2\

.PHONY : clean install windows sprite
clean :
	rm -rf $(BUILD_DIR)
	rm $(BIN) || rm $(BIN).exe
	
sprite:
	cd sprite
	jot diffuse $(SPRS)
	cp sprite/images_diffuse.h src/images_diffuse.h
	cp sprite/images_diffuse.c src/images_diffuse.c
	cd ..

install : $(BIN)
	cp $(BIN) $(INSTALL_PATH)/bin
    
windows : $(OBJS:%=$(BUILD_DIR)/%)
	$(CC) $^ -o shader-test-windows.exe $(CFLAGS) -mwindows