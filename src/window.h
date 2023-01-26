#ifndef __GUM__VULKAN__WINDOW__
#define __GUM__VULKAN__WINDOW__

#ifdef WAY_PROT
#elif X11_PROT
#endif

#include <vulkan/vulkan.h>
#include <stdint.h>

//structures

#ifdef WAY_PROT

#include <wayland-client.h>
#include <vulkan/vInit_wayland.h>
struct window_window{
    struct wl_display *display;
    struct wl_registry *registry;
    
    struct wl_compositor *compositor;
    struct wl_surface *surface;
    
    int32_t width, height;
};

#elif X11_PROT

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
struct window_window{
    GLFWwindow *window;
    int32_t width, height;
};

#else

<<<<select a WM>>>>>>

#endif


//function prototypes

int32_t window_initWindow(struct window_window *windowParam, int32_t widthParam, int32_t heightParam, int8_t *nameParam);
int32_t window_closeWindowEvent(struct window_window *windowParam);
int32_t window_destroyWindow(struct window_window *windowParam);
int32_t window_getRequiredInstanceExtentions(const char ***extensions, uint32_t *extensionsCount);
int32_t window_getFrameBufferSize(struct window_window *window, int32_t *bufferWidth, int32_t *bufferHeight);
void window_pollEvents();

VkResult window_createSurface(VkSurfaceKHR *surface, struct window_window *windowParam, VkInstance instance);

#endif
