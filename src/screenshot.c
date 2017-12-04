#include <GLFW/glfw3.h>

#include "graphics.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void take_screenshot(int N) {
    if(N > 9999) N = 9999;
    if(N < 0) N = 0;
    
    char buf[18];
    
    snprintf(buf, 18, "scr/frame%d.png", N);
    
    printf("saving to %s\n", buf);
    
    char *pixels = malloc(sizeof(char) * get_cam_width() * get_cam_height() * 3);
    
    puts("got pixels");
    
    glReadPixels(0, 0, get_cam_width(), get_cam_height(), GL_RGB, GL_UNSIGNED_BYTE, pixels);
    
    puts("read pixels");
    
    stbi_write_png("hello.png", 10, 10, 3, pixels, 0);
    free(pixels);
}