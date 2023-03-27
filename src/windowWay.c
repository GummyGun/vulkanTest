#include "window.h"
#include "xdg-shell-client-protocol.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*------------------xdgFunctionality------------------*/

#include "xdg-shell-protocol.c"

/*include here since it will allways be used by windowWay and makes the makefile way cleaner than generating the standalone obj file*/

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

static void s_nop();

static void s_registryHandleGlobal(void *data, struct wl_registry *wlRegistry, uint32_t id, const char *interface, uint32_t version) ;
static void s_registryHandleGlobalRemove(void *data, struct wl_registry *registry, uint32_t name);

static void s_xdgSurfaceHandleConfigure(void *data, struct xdg_surface *surface, uint32_t serial);

/*------------------    globals    ------------------*/

//defines for portability
char *const extensionsWayland[] = {VK_KHR_SURFACE_EXTENSION_NAME,VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME};
//{"VK_KHR_surface", "VK_KHR_wayland_surface"};
//{VK_KHR_SURFACE_EXTENSION_NAME,VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME};
const uint32_t extensionsWaylandCount = sizeof(extensionsWayland)/sizeof(int8_t*);


/*------------------implementations------------------*/

/* ================================= extern methods ================================== */
/*                                                                                     */
/*                                                                                     */
/*                _                     __                  _   _                      */
/*               | |                   / _|                | | (_)                     */
/*       _____  _| |_ ___ _ __ _ __   | |_ _   _ _ __   ___| |_ _  ___  _ __  ___      */
/*      / _ \ \/ / __/ _ \ '__| '_ \  |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|     */
/*     |  __/>  <| ||  __/ |  | | | | | | | |_| | | | | (__| |_| | (_) | | | \__ \     */
/*      \___/_/\_\\__\___|_|  |_| |_| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/     */
/*                                                                                     */
/*                                                                                     */
/*                                                                                     */
/* ================================= extern methods ================================== */

int32_t 
window_initWindow(struct window_window *windowParam, int32_t widthParam, int32_t heightParam, int8_t *nameParam){
    static const 
    struct wl_registry_listener
    registryListener = {
        .global = s_registryHandleGlobal,
        .global_remove = s_registryHandleGlobalRemove,
    };

    windowParam->width=widthParam;
    windowParam->height=heightParam;
    
    windowParam->display = wl_display_connect(NULL);
    windowParam->registry = wl_display_get_registry(windowParam->display);
    
    if(!(windowParam->display &&  windowParam->registry)){
        fprintf(stderr, "Error: Establishing wayland connection\n");
        return 1;
    }
    
	wl_registry_add_listener(windowParam->registry, &registryListener, windowParam);
	wl_display_roundtrip(windowParam->display);
    windowParam->surface = wl_compositor_create_surface(windowParam->compositor);
    
    static const 
    struct xdg_surface_listener 
    xdgSurfaceListener = {
        .configure = s_xdgSurfaceHandleConfigure
    };
    
    windowParam->xdgSurface = xdg_wm_base_get_xdg_surface(windowParam->xdgWmBase, windowParam->surface);
    xdg_surface_add_listener(windowParam->xdgSurface, &xdgSurfaceListener, windowParam);
    
    static const 
    struct xdg_toplevel_listener  
    xdgToplevelListener = {
        .configure = s_nop,
        .close = s_nop
    };
    
    windowParam->xdgToplevel = xdg_surface_get_toplevel(windowParam->xdgSurface);
    xdg_toplevel_add_listener(windowParam->xdgToplevel, &xdgToplevelListener, windowParam);
    
    xdg_toplevel_set_title(windowParam->xdgToplevel, "VulkanTest");
    wl_surface_commit(windowParam->surface);
    
    /*
    printf("surfacePtr: %p\n", windowParam->surface);
    
    int32_t wait=0;
    while(wait++<1000000000){
        if((wait%100000000)==0){
            printf("a");
        }
    }
    printf("\n");
    */
    
    return 0;
}


int32_t 
window_closeWindowEvent(struct window_window *windowParam){
    static int state = 10000;
    
    printf("%d\n", state);
    return state--;
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
    *bufferWidth = window->width;
    *bufferHeight = window->height;
    /*
    glfwGetFramebufferSize(window->window, bufferWidth, bufferHeight);
    */
    return 1;
}

int32_t
window_pollEvents(struct window_window *window){
    //event loop events
    return 0;
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

/* =============================== static methods =============================== */
/*                                                                                */
/*                                                                                */
/*          _        _   _         __                  _   _                      */
/*         | |      | | (_)       / _|                | | (_)                     */
/*      ___| |_ __ _| |_ _  ___  | |_ _   _ _ __   ___| |_ _  ___  _ __  ___      */
/*     / __| __/ _` | __| |/ __| |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|     */
/*     \__ \ || (_| | |_| | (__  | | | |_| | | | | (__| |_| | (_) | | | \__ \     */
/*     |___/\__\__,_|\__|_|\___| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/     */
/*                                                                                */
/*                                                                                */
/*                                                                                */
/* =============================== static methods =============================== */

static
void
s_nop(){
}

static 
void
s_registryHandleGlobal(void *data, struct wl_registry *wlRegistry, uint32_t id, const char *interface, uint32_t version) {
    struct window_window *window = (struct window_window*)data;
    
    if(strcmp(interface, wl_compositor_interface.name) == 0) {
        window->compositor = wl_registry_bind(wlRegistry, id, &wl_compositor_interface, version);
        printf("compositor was saved\n");
    }else if(strcmp(interface, xdg_wm_base_interface.name) == 0) {
        window->xdgWmBase = wl_registry_bind(wlRegistry, id, &xdg_wm_base_interface, version);
        printf("xdgWmBase was saved\n");
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

static 
void
s_xdgSurfaceHandleConfigure(void *data, struct xdg_surface *surface, uint32_t serial){
	//VulkanExampleBase *base = (VulkanExampleBase *) data;
	xdg_surface_ack_configure(surface, serial);
	//base->configured = true;
}

static
void
s_xdgToplevelHandleConfigure(void *data, struct xdg_toplevel *toplevel, int32_t width, int32_t height, struct wl_array *states){
    
}

