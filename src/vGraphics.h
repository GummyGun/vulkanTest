#ifndef __GUM__VULKAN__VGRAPHICS__
#define __GUM__VULKAN__VGRAPHICS__

#include "vulkan.h"

#include <vulkan/vulkan.h>
#include <stdint.h>

//structures
struct vGraph_syncObjects{
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
};

struct vGraph_frameBufferDetails{
    VkFramebuffer *frameBuffers;
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
    VkCommandBuffer commandBuffer;
    
    struct vGraph_syncObjects syncObjects;
};

//functions

int32_t vGraph_initPipeline(struct vGraph_pipeline *graphicsPipeline, struct vulkan_graphicsStruct *graphicsPacket);
void vGraph_destroyPipeline(struct vGraph_pipeline *graphicsPipeline, struct vulkan_graphicsStruct *graphicsPacket);
int32_t vGraph_drawFrame(struct vGraph_pipeline *graphicsPipeline, struct vulkan_graphicsStruct *graphicsPacket);

#endif
