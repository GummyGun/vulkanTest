#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "window.h"

int main(){
    printf("hola 2\n");
    
    struct window *window;
    
    if(!window_createWindow(&window, 300, 300, "hola")){
        printf("[Window] error creating the window\n");
        exit(0);
    }
    

    while(!window_closeWindow(window)){
        glfwPollEvents();
    }
    
}
