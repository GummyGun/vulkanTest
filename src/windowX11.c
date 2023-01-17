#include "window.h"
#include <stdlib.h>

/*

int32_t window_createWindow(struct window *windowParam, int32_t widthParam, int32_t heightParam, int8_t *nameParam);
int32_t window_closeWindowEvent(struct window *windowParam);
int32_t window_deleteWindow(struct window_window *windowParam);
int32_t window_getRequiredInstanceExtentions(const char ***extensions, uint32_t *extensionsCount);
int32_t window_getFrameBufferSize(struct window_window *window, int32_t *bufferWidth, int32_t *bufferHeight);

*/

int32_t 
window_initWindow(struct window_window *windowParam, int32_t widthParam, int32_t heightParam, int8_t *nameParam){
    windowParam->width=widthParam;
    windowParam->height=heightParam;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    windowParam->window = glfwCreateWindow(widthParam, heightParam, nameParam, NULL, NULL);
    return 0;
}


int32_t 
window_closeWindowEvent(struct window_window *windowParam){
    return glfwWindowShouldClose(windowParam->window);
}

int32_t
window_destroyWindow(struct window_window *windowParam){
    glfwDestroyWindow(windowParam->window);
    glfwTerminate();
}

int32_t 
window_getRequiredInstanceExtentions(const char ***extensions, uint32_t *extensionsCount){
    *extensions = glfwGetRequiredInstanceExtensions(extensionsCount);
    return 0;
}

int32_t
window_getFrameBufferSize(struct window_window *window, int32_t *bufferWidth, int32_t *bufferHeight){
    glfwGetFramebufferSize(window->window, bufferWidth, bufferHeight);
    return 0;
}

VkResult
window_createSurface(VkSurfaceKHR *surface, struct window_window *windowParam, VkInstance instance){
    return glfwCreateWindowSurface(instance, windowParam->window, NULL, surface);
}

