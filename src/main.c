#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "window.h"
#include "vulkan.h"

int main(){
    printf("hola2\n");
    
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    
    struct window *window;
    
    if(window_createWindow(&window, 300, 300, "hola")){
        printf("[Window] error creating the window\n");
        exit(0);
    }
    
    if(vulkan_initVulkan(&instance, &physicalDevice)){
        fprintf(stderr, "Error creating vulkan\n");
        assert(0 && "Error creating vulkan");
    }
    
    while(!window_closeWindow(window)){
        glfwPollEvents();
    }
    
    if(vulkan_deleteVulkan(&instance, &physicalDevice)){
        fprintf(stderr, "Error deleting vulkan\n");
        assert(0 && "Error deleting all vulkan");
    }
    
    printf("adios\n");
    
}
