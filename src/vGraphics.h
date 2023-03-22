#ifndef __GUM__VULKAN__VGRAPHICS__
#define __GUM__VULKAN__VGRAPHICS__

#include "vInit.h"

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>

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
    struct vGraph_semaphoreDetails renderFinishedSemaphoreArray;
    struct vGraph_fenceDetails inFlightFenceArray;
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
    int32_t currentFrame;
    uint64_t totalFrames;
    
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    VkRenderPass renderPass;
    VkPipelineLayout layout;
    VkPipeline graphicsPipeline;
    struct vGraph_frameBufferDetails frameBufferArray;
    
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    
    VkCommandPool commandPool;
    struct vGraph_commandBufferDetails commandBufferArray;
    
    struct vGraph_syncObjects syncObjects;
};

struct vGraph_simpleVertex{
    vec2 position;
    vec3 color;
};

struct vGraph_vertexDescription{
    VkVertexInputAttributeDescription attributes[2];
};

//functions

int32_t vGraph_initPipeline(struct vGraph_pipeline *graphicsPipeline, struct vInit_graphicsStruct *graphicsPacket);
void vGraph_destroyPipeline(struct vGraph_pipeline *graphicsPipeline, struct vInit_graphicsStruct *graphicsPacket);
int32_t vGraph_drawFrame(struct vInit_graphicsStruct *graphicsPacket, struct vGraph_pipeline *graphicsPipeline);
void vGraph_waitForIdle(struct vInit_graphicsStruct *graphicsPacket);

#endif
