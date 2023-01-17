#ifndef __GUM__VULKAN__VGRAPHICS__
#define __GUM__VULKAN__VGRAPHICS__

#include "vulkan.h"

#include <vulkan/vulkan.h>
#include <stdint.h>

//structures

struct vGraph_pipeline{
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    VkRenderPass renderPass;
    VkPipelineLayout layout;
    VkPipeline graphicsPipeline;
};

//functions

int32_t vGraph_initPipeline(struct vGraph_pipeline *graphicsPipeline, struct vulkan_graphicsStruct *graphicsPacket);
void vGraph_destroyPipeline(struct vGraph_pipeline *graphicsPipeline, struct vulkan_graphicsStruct *graphicsPacket);

#endif
