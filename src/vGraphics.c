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

//int32_t vGraph_createPipeline(struct vGraph_pipeline *graphicsPipeline, struct vulkan_graphicsStruct *graphicsPacket);
//int32_t vGraph_createPipelineLayout(VkPipelineLayout **pipelineLayout, VkDevice device);

//void vGraph_deletePipelineLayout(VkPipelineLayout **pipelineLayout, VkDevice device);
//void vGraph_deletePipeline(struct vGraph_pipeline *graphicsPipeline, VkDevice device);

//statics

static int32_t s_createShaderModule(VkShaderModule *shaderModule, const struct utils_file *shaderSPVFile, VkDevice device);
static void s_deleteShaderModule(VkShaderModule *shaderModule, VkDevice device);



/*------------------    globals    ------------------*/

/*------------------implementations------------------*/

int32_t
vGraph_createPipeline(struct vGraph_pipeline *graphicsPipeline, struct vulkan_graphicsStruct *graphicsPacket){
    printf("Creating Pipeline\n");
    struct utils_file vertShaderSPV, fragShaderSPV;
    if(utils_openFile(&vertShaderSPV, "res/shaders/shader.vert.spv")){
        return 1;
    }
    if(utils_openFile(&fragShaderSPV, "res/shaders/shader.frag.spv")){
        return 1;
    }
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

    VkPipelineShaderStageCreateInfo shaderStages[2] = {};
    shaderStages->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages->stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages->module = graphicsPipeline->vertShaderModule;
    shaderStages->pName = "main";
    
    (shaderStages+1)->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    (shaderStages+1)->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    (shaderStages+1)->module = graphicsPipeline->fragShaderModule;
    (shaderStages+1)->pName = "main";
    
    VkDynamicState const dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    const int32_t dynamicStatesCount = sizeof(dynamicStates)/sizeof(VkDynamicState);
    
    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
    dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount = dynamicStatesCount;
    dynamicStateCreateInfo.pDynamicStates = dynamicStates;
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = NULL; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount ;
    vertexInputInfo.pVertexAttributeDescriptions = NULL; // Optional
    
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = {};
    inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;
    
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)graphicsPacket->swapchainDetails.extent.width;
    viewport.height = (float)graphicsPacket->swapchainDetails.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    
    VkRect2D scissor = {};
    scissor.offset = (VkOffset2D){0, 0};
    scissor.extent = graphicsPacket->swapchainDetails.extent;
    
    VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
    viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.viewportCount = 1;
    viewportStateCreateInfo.scissorCount = 1;
    
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    
    
    rasterizer.lineWidth = 1.0f;
    
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
    
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = NULL; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional
    
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
    
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional
    
    if(vGraph_createPipelineLayout(&(graphicsPipeline->layout), graphicsPacket->device)){
        return 1;
    }
    
    
    return 0;
}

void
vGraph_deletePipeline(struct vGraph_pipeline *graphicsPipeline, struct vulkan_graphicsStruct *graphicsPacket){
    printf("deleting the pipeline\n");
    vGraph_deletePipelineLayout(&(graphicsPipeline->layout), (graphicsPacket->device));
    s_deleteShaderModule(&(graphicsPipeline->vertShaderModule), (graphicsPacket->device));
    s_deleteShaderModule(&(graphicsPipeline->fragShaderModule), (graphicsPacket->device));
}

int32_t
vGraph_createPipelineLayout(VkPipelineLayout *pipelineLayout, VkDevice device){
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 0; // Optional
    pipelineLayoutCreateInfo.pSetLayouts = NULL; // Optional
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutCreateInfo.pPushConstantRanges = NULL; // Optional
    
    if(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL, pipelineLayout) != VK_SUCCESS) {
        fprintf(stderr, "Error: creating pipeline\n");
        return 1;
    }
    return 0;
}

void
vGraph_deletePipelineLayout(VkPipelineLayout *pipelineLayout, VkDevice device){
    vkDestroyPipelineLayout(device, *pipelineLayout, NULL);
    *pipelineLayout=NULL;
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

static
void
s_deleteShaderModule(VkShaderModule *shaderModule, VkDevice device){
    vkDestroyShaderModule(device, *shaderModule, NULL);
    *shaderModule = NULL;
}
