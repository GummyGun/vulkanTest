#include "window.h"
#include <stdlib.h>
#include <stdio.h>

/*
int32_t window_createWindow(struct window *windowParam, int32_t widthParam, int32_t heightParam, int8_t *nameParam);
int32_t window_closeWindowEvent(struct window *windowParam);
int32_t window_deleteWindow(struct window_window *windowParam);
int32_t window_getRequiredInstanceExtentions(const char ***extensions, uint32_t *extensionsCount);
int32_t window_getFrameBufferSize(struct window_window *window, int32_t *bufferWidth, int32_t *bufferHeight);
*/

char *const extensionsWayland[] = {"VK_KHR_surface","VK_KHR_wayland_surface"};
const uint32_t extensionsWaylandCount = sizeof(extensionsWayland)/sizeof(int8_t*);



int32_t 
window_initWindow(struct window_window *windowParam, int32_t widthParam, int32_t heightParam, int8_t *nameParam){
    windowParam->display = wl_display_connect(NULL);
    windowParam->registry = wl_display_get_registry(windowParam->display);
    printf("%s\n", wl_compositor_interface.name);
    if(!windowParam->display){
        return 1;
    }
    printf("Connection established\n");
    /*
    windowParam->width=widthParam;
    windowParam->height=heightParam;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    windowParam->window = glfwCreateWindow(widthParam, heightParam, nameParam, NULL, NULL);
    */
    return 0;
}


int32_t 
window_closeWindowEvent(struct window_window *windowParam){
    //return glfwWindowShouldClose(windowParam->window);
    return 1;
}

int32_t
window_destroyWindow(struct window_window *windowParam){
    /*
    glfwDestroyWindow(windowParam->window);
    glfwTerminate();
    */
    return 1;
}

int32_t 
window_getRequiredInstanceExtentions(const char ***extensions, uint32_t *extensionsCount){
    *extensions = (const char **)extensionsWayland;
    *extensionsCount = extensionsWaylandCount;
    /*
    *extensions = glfwGetRequiredInstanceExtensions(extensionsCount);
    */
    return 0;
}

int32_t
window_getFrameBufferSize(struct window_window *window, int32_t *bufferWidth, int32_t *bufferHeight){
    /*
    glfwGetFramebufferSize(window->window, bufferWidth, bufferHeight);
    */
    return 1;
}

VkResult
window_createSurface(VkSurfaceKHR *surface, struct window_window *windowParam, VkInstance instance){
    //return glfwCreateWindowSurface(instance, windowParam->window, NULL, surface);
    return 1;
}

