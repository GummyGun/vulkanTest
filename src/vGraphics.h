#ifndef __GUM__VULKAN__VGRAPHICS__
#define __GUM__VULKAN__VGRAPHICS__

#include "vulkan.h"

#include <vulkan/vulkan.h>
#include <stdint.h>

//structures

struct vGraph_pipeline{
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};

//functions

int32_t vGraph_createPipeline(struct vulkan_graphicsStruct *graphicsPacket, struct vGraph_pipeline *graphicsPipeline);


#endif
