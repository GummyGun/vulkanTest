#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "window.h"
#include "vInit.h"
#include "vGraphics.h"



int main(){
    printf("%s\n\n", VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    printf("Startup\n");
    
    struct window_window window = {};
    struct vInit_graphicsStruct vulkanStruct = {};
    struct vGraph_pipeline vulkanPipeline = {};
    
    vulkanStruct.debugMode = 1;
    
    
    if(window_initWindow(&window, 900, 900, "vulkanTest")){
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
        if(window_pollEvents(&window)){
            break;
        }
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

