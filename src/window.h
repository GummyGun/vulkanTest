#ifndef __GUM__VULKAN__WINDOW__
#define __GUM__VULKAN__WINDOW__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdint.h>

//structures

struct window_window{
    GLFWwindow *window;
    int32_t width, height;
};

//function prototypes

int32_t window_createWindow(struct window_window *windowParam, int32_t widthParam, int32_t heightParam, int8_t *nameParam);
int32_t window_closeWindowEvent(struct window_window *windowParam);
int32_t window_deleteWindow(struct window_window *windowParam);
int32_t window_getRequiredInstanceExtentions(const char ***extensions, uint32_t *extensionsCount);
int32_t window_getFrameBufferSize(struct window_window *window, int32_t *bufferWidth, int32_t *bufferHeight);

int32_t window_createSurface(VkSurfaceKHR *surface, struct window_window *windowParam, VkInstance instance);

#endif
