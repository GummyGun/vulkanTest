#ifndef __GUM__VULKAN__VGRAPHICS__
#define __GUM__VULKAN__VGRAPHICS__

#include "vulkan.h"

#include <vulkan/vulkan.h>
#include <stdint.h>

//structures

struct vGraph_semaphoreDetails{
    VkSemaphore *semaphores;
    int32_t count;
};

struct vGraph_fenceDetails{
    VkFence *fences;
    int32_t count;
};

struct vGraph_syncObjects{
    struct vGraph_semaphoreDetails imageAvailableSemaphoreArray;
    VkSemaphore imageAvailableSemaphore;
    struct vGraph_semaphoreDetails renderFinishedSemaphoreArray;
    VkSemaphore renderFinishedSemaphore;
    struct vGraph_fenceDetails inFlightFenceArray;
    VkFence inFlightFence;
};

struct vGraph_frameBufferDetails{
    VkFramebuffer *frameBuffers;
    int32_t count;
};

struct vGraph_commandBufferDetails{
    VkCommandBuffer *commandBuffers;
    int32_t count;
};

struct vGraph_pipeline{
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    VkRenderPass renderPass;
    VkPipelineLayout layout;
    VkPipeline graphicsPipeline;
    struct vGraph_frameBufferDetails frameBufferArray;
    
    VkCommandPool commandPool;
    struct vGraph_commandBufferDetails commandBufferArray;
    
    struct vGraph_syncObjects syncObjects;
};

//functions

int32_t vGraph_initPipeline(struct vGraph_pipeline *graphicsPipeline, struct vInit_graphicsStruct *graphicsPacket);
void vGraph_destroyPipeline(struct vGraph_pipeline *graphicsPipeline, struct vInit_graphicsStruct *graphicsPacket);
int32_t vGraph_drawFrame(struct vInit_graphicsStruct *graphicsPacket, struct vGraph_pipeline *graphicsPipeline);
void vGraph_waitForIdle(struct vInit_graphicsStruct *graphicsPacket);

#endif
