#ifndef __GUM__VULKAN__WINDOW__
#define __GUM__VULKAN__WINDOW__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdbool.h>

struct window{
    GLFWwindow *window;
    int32_t width, height;
};

int32_t window_createWindow(struct window **windowParam, int32_t widthParam, int32_t heightParam, int8_t *nameParam);
int32_t window_closeWindow(struct window *windowParam);
int32_t window_getRequiredInsanceExtentions(const char ***extensions, uint32_t *extensionsCount);

#endif
