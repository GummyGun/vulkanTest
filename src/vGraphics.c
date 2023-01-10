#include "vulkan.h"
#include "vGraphics.h"
#include "utils.h"

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdio.h>

/*------------------defines------------------*/

/*------------------  enums  ------------------*/

/*------------------  stucts  ------------------*/

/*------------------prototipes------------------*/

//int32_t vGraph_createPipeline(struct vulkan_graphicsStruct *graphicsPacket);

//int32_t vGraph_deletePipeline(struct vulkan_graphicsStruct *graphicsPacket);

//statics

static int32_t s_createShaderModule(VkShaderModule *shaderModule, const struct utils_file *shaderSPVFile, VkDevice device);

/*------------------    globals    ------------------*/

/*------------------implementations------------------*/

int32_t
vGraph_createPipeline(struct vulkan_graphicsStruct *graphicsPacket, struct vGraph_pipeline *graphicsPipeline){
    printf("Creating Pipeline\n");
    struct utils_file vertShaderSPV, fragShaderSPV;
    utils_openFile(&vertShaderSPV, "res/shaders/shader.vert.spv");
    utils_openFile(&fragShaderSPV, "res/shaders/shader.frag.spv");
    
    if(s_createShaderModule(&(graphicsPipeline->vertShaderModule), &vertShaderSPV, (graphicsPacket->device))){
        return 1;
    };
    
    if(s_createShaderModule(&(graphicsPipeline->fragShaderModule), &fragShaderSPV, (graphicsPacket->device))){
        return 1;
    };
    
    if(utils_closeFile(&vertShaderSPV)){
        return 1;
    }
    if(utils_closeFile(&fragShaderSPV)){
        return 1;
    }
    return 0;
}

//statics

static
int32_t
s_createShaderModule(VkShaderModule *shaderModule, const struct utils_file *shaderSPVFile, VkDevice device){
    VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = shaderSPVFile->size;
    shaderModuleCreateInfo.pCode = (uint32_t*)(shaderSPVFile->content);
    if(vkCreateShaderModule(device, &shaderModuleCreateInfo, NULL, shaderModule)){
        fprintf(stderr, "Error: creating shader module\n");
        return 1;
    }
    return 0;
}
