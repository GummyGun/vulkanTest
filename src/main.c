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
    struct vInit_graphicsStruct vulkanStruct = {};
    struct vGraph_pipeline vulkanPipeline = {};
    
    vulkanStruct.debugMode = 1;
    
    if(window_initWindow(&window, 1200, 900, "vulkanTest")){
        fprintf(stderr, "[Window] Error: Creating the window\n");
        assert(0 && "Error creating window");
    }
    
    if(vInit_initVulkan(&vulkanStruct, &window)){
        fprintf(stderr, "[Vulkan] Error: Creating vulkan\n");
        assert(0 && "Error creating vulkan");
    }
    
    
    if(vGraph_initPipeline(&vulkanPipeline, &vulkanStruct)){
        fprintf(stderr, "[vGraphics] Error: Creating pipeline\n");
        assert(0 && "Error creating pipeline");
    }
    
    printf("------------------------------------------------\n\n------------------------------------------------\n");
    
    while(window_closeWindowEvent(&window)){
        window_pollEvents();
        if(vGraph_drawFrame(&vulkanStruct, &vulkanPipeline)){
            fprintf(stderr, "[vGraphics] Error: Drawing triangle\n");
            assert(0 && "Error drawing triangle");
        }
    }
    
    vGraph_waitForIdle(&vulkanStruct);
    
    printf("------------------------------------------------\n\n------------------------------------------------\n");
    
    vGraph_destroyPipeline(&vulkanPipeline, &vulkanStruct);
    vInit_destroyVulkan(&vulkanStruct);
    window_destroyWindow(&window);
    
    printf("%ld frames were drawn\n", vulkanPipeline.totalFrames);
    printf("Bye bye\n");
    
}
