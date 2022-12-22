#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "window.h"
#include "vulkan.h"

int main(){
    printf("Startup\n");
    
    VkInstance instance;
    VkDevice device;
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;
    
    struct window_window window;
    
    if(window_createWindow(&window, 300, 300, "hola")){
        fprintf(stderr, "[Window] error creating the window\n");
        assert(0 && "Error creating window");
    }
    
    if(vulkan_initVulkan(&instance, &device, &graphicsQueue, &surface, window.window)){
        fprintf(stderr, "Error creating vulkan\n");
        assert(0 && "Error creating vulkan");
    }
    
    while(!window_closeWindowEvent(&window)){
        glfwPollEvents();
    }
    
    printf("------------------------------------------------\n------------------------------------------------\n");
    
    vulkan_deleteVulkan(&instance, &surface, &device);
    window_deleteWindow(&window);
    
    printf("Bye bye\n");
    
}
