#ifndef __GUM__VULKAN__VGRAPHICS__
#define __GUM__VULKAN__VGRAPHICS__

#include "vulkan.h"

#include <vulkan/vulkan.h>
#include <stdint.h>

//structures

struct vGraph_pipeline{
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    VkPipelineLayout layout;
    
};

//functions

int32_t vGraph_createPipeline(struct vGraph_pipeline *graphicsPipeline, struct vulkan_graphicsStruct *graphicsPacket);
int32_t vGraph_createPipelineLayout(VkPipelineLayout *pipelineLayout, VkDevice device);

void vGraph_deletePipelineLayout(VkPipelineLayout *pipelineLayout, VkDevice device);
void vGraph_deletePipeline(struct vGraph_pipeline *graphicsPipeline, struct vulkan_graphicsStruct *graphicsPacket);

#endif
