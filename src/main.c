#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "window.h"
#include "vulkan.h"
#include "vGraphics.h"



int main(){
    printf("Startup\n");
    
    struct window_window window = {};
    struct vulkan_graphicsStruct vulkanStruct = {};
    struct vGraph_pipeline vulkanPipeline = {};
    
    if(window_createWindow(&window, 300, 300, "vulkanTest")){
        fprintf(stderr, "[Window] error creating the window\n");
        assert(0 && "Error creating window");
    }
    
    if(vulkan_initVulkan(&vulkanStruct, &window)){
        fprintf(stderr, "Error: creating vulkan\n");
        assert(0 && "Error creating vulkan");
    }
    
    if(vGraph_createPipeline(&vulkanPipeline, &vulkanStruct)){
        fprintf(stderr, "Error: creating pipeline\n");
        assert(0 && "Error creating pipeline");
    }
    
    while(!window_closeWindowEvent(&window)){
        glfwPollEvents();
    }
    
    printf("------------------------------------------------\n\n------------------------------------------------\n");
    
    vGraph_deletePipeline(&vulkanPipeline, &vulkanStruct);
    vulkan_deleteVulkan(&vulkanStruct);
    window_deleteWindow(&window);
    
    printf("Bye bye\n");
    
}
