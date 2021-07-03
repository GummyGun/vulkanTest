#ifndef __GUM__WINDOW__
#define __GUM__WINDOW__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdbool.h>

struct window{
    GLFWwindow *window;
    int width, height;
};

bool window_createWindow(struct window **windowParam, int widthParam, int heightParam, char *nameParam);
bool window_closeWindow(struct window *windowParam);

#endif
