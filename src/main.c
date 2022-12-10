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
    
    struct window *window;
    
    if(window_createWindow(&window, 300, 300, "hola")){
        printf("[Window] error creating the window\n");
        exit(0);
    }
    
    if(vulkan_createInstance(&instance)){
        fprintf(stderr, "Error creating instance\n");
        assert(0 && "Error creating instance");
    }
    
    while(!window_closeWindow(window)){
        glfwPollEvents();
    }
    
    if(vulkan_deleteInstance(instance)){
        fprintf(stderr, "Error deleting instance\n");
        assert(0 && "Error deleting instance");
    }
    
    printf("adios\n");
    
}
