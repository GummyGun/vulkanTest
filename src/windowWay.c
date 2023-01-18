/*
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <wayland-client.h>

int
main(int argc, char *argv[]){
	    struct wl_display *display = wl_display_connect(NULL);
	    struct wl_registry *registry = wl_display_get_registry(display);
    struct wl_compositor *compositor = NULL;
    struct wl_surface *surface = NULL;
    printf("%s\n", wl_compositor_interface.name);
    printf("after: \ncompositor: %p\nsurface: %p\n", compositor, surface);
    surface = wl_compositor_create_surface(compositor);
    printf("after: \ncompositor: %p\nsurface: %p\n", compositor, surface);
	return 0;
}
*/
#include "window.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*------------------defines------------------*/

/*------------------  enums  ------------------*/

/*------------------  stucts  ------------------*/

/*------------------prototipes------------------*/

//int32_t window_createWindow(struct window *windowParam, int32_t widthParam, int32_t heightParam, int8_t *nameParam);
//int32_t window_closeWindowEvent(struct window *windowParam);
//int32_t window_deleteWindow(struct window_window *windowParam);
//int32_t window_getRequiredInstanceExtentions(const char ***extensions, uint32_t *extensionsCount);
//int32_t window_getFrameBufferSize(struct window_window *window, int32_t *bufferWidth, int32_t *bufferHeight);

//statics

static void s_registryHandleGlobal(void *data, struct wl_registry *wlRegistry, uint32_t id, const char *interface, uint32_t version) ;
static void s_registryHandleGlobalRemove(void *data, struct wl_registry *registry, uint32_t name);

/*------------------    globals    ------------------*/

char *const extensionsWayland[] = {"VK_KHR_surface","VK_KHR_wayland_surface"};
const uint32_t extensionsWaylandCount = sizeof(extensionsWayland)/sizeof(int8_t*);

static const 
struct wl_registry_listener
registryListener = {
	.global = s_registryHandleGlobal,
	.global_remove = s_registryHandleGlobalRemove,
};


/*------------------implementations------------------*/


int32_t 
window_initWindow(struct window_window *windowParam, int32_t widthParam, int32_t heightParam, int8_t *nameParam){
    windowParam->display = wl_display_connect(NULL);
    windowParam->registry = wl_display_get_registry(windowParam->display);
    
    if(!(windowParam->display &&  windowParam->registry)){
        fprintf(stderr, "Error: Establishing wayland connection\n");
        return 1;
    }
    
	wl_registry_add_listener(windowParam->registry, &registryListener, windowParam);
	wl_display_roundtrip(windowParam->display);
    windowParam->surface = wl_compositor_create_surface(windowParam->compositor);
    printf("surfacePtr: %p\n", windowParam->surface);
    
    
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
    return 0;
}

int32_t
window_getFrameBufferSize(struct window_window *window, int32_t *bufferWidth, int32_t *bufferHeight){
    /*
    glfwGetFramebufferSize(window->window, bufferWidth, bufferHeight);
    */
    return 1;
}

void
window_pollEvents(){
    //event loop events
}

VkResult
window_createSurface(VkSurfaceKHR *surface, struct window_window *windowParam, VkInstance instance){
    VkWaylandSurfaceCreateInfoKHR waylandSurfaceCreateInfo = {};
    waylandSurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    waylandSurfaceCreateInfo.display = windowParam->display;
    waylandSurfaceCreateInfo.surface = windowParam->surface;
    return vkCreateWaylandSurfaceKHR(instance, &waylandSurfaceCreateInfo, NULL, surface);
    //return 1;
}

/* ----------------------- static methods ----------------------- */

static 
void
s_registryHandleGlobal(void *data, struct wl_registry *wlRegistry, uint32_t id, const char *interface, uint32_t version) {
    struct window_window *window = (struct window_window*)data;
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        window->compositor = wl_registry_bind(wlRegistry, id, &wl_compositor_interface, version);
        printf("compositor was saved\n");
    }
    /*
    else{
        printf("interface: '%s', version: %d, id: %d\n", interface, version, id);
    }
    */
}

static 
void
s_registryHandleGlobalRemove(void *data, struct wl_registry *registry, uint32_t name){
	// This space deliberately left blank
}

