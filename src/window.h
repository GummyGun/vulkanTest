#ifndef __GUM__VULKAN__WINDOW__
#define __GUM__VULKAN__WINDOW__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdbool.h>

//structures

struct window_window{
    GLFWwindow *window;
    int32_t width, height;
};

//function prototypes

int32_t window_createWindow(struct window_window *windowParam, int32_t widthParam, int32_t heightParam, int8_t *nameParam);
int32_t window_closeWindowEvent(struct window_window *windowParam);
int32_t window_deleteWindow(struct window_window *windowParam);
int32_t window_getRequiredInsanceExtentions(const char ***extensions, uint32_t *extensionsCount);

#endif
