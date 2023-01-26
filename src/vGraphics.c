#include "vulkan.h"
#include "vGraphics.h"
#include "utils.h"

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*------------------defines------------------*/

/*------------------  enums  ------------------*/

/*------------------  stucts  ------------------*/

/*------------------prototipes------------------*/

//int32_t vGraph_initPipeline(struct vGraph_pipeline *graphicsPipeline, struct vInit_graphicsStruct *graphicsPacket);
//void vGraph_destroyPipeline(struct vGraph_pipeline *graphicsPipeline, struct vInit_graphicsStruct *graphicsPacket);
//int32_t vGraph_drawFrame(struct vInit_graphicsStruct *graphicsPacket, struct vGraph_pipeline *graphicsPipeline);

//void vGraph_waitForIdle(struct vInit_graphicsStruct *graphicsPacket);

/* window ->instance -> surface -> physicalDevice -> device -> Queue -> swapchain -> swapchainDetails -> images -> imageViews */
/* renderPass */

//statics

static int32_t s_createPipeline(struct vGraph_pipeline *graphicsPipeline, VkDevice device, struct vInit_swapchainDetails *swapchainDetails);
static void s_deletePipeline(struct vGraph_pipeline *graphicsPipeline, VkDevice device);

static int32_t s_createShaderModule(VkShaderModule *shaderModule, const char *shaderFileName, VkDevice device);
static void s_deleteShaderModule(VkShaderModule *shaderModule, VkDevice device);

static int32_t s_createPipelineLayout(VkPipelineLayout *pipelineLayout, VkDevice device);
static void s_deletePipelineLayout(VkPipelineLayout *pipelineLayout, VkDevice device);

static int32_t s_createRenderPass(VkRenderPass *renderPass, VkDevice device, const VkFormat *const swapchainImageFormat);
static void s_deleteRenderPass(VkRenderPass *renderPass, VkDevice device);

static int32_t s_createFrameBuffer(struct vGraph_frameBufferDetails *frameBufferArray, VkDevice device, struct vInit_swapchainDetails *swapchainDetails, struct vInit_imageViewDetails *imageViewArray, VkRenderPass renderPass);
static void s_deleteFrameBuffer(struct vGraph_frameBufferDetails *frameBufferArray, VkDevice device);

static int32_t s_createCommandPool(VkCommandPool *commandPool, struct vInit_queueIndices *queueIndices, VkDevice device);//to change
static void s_deleteCommandPool(VkCommandPool *commandPool, VkDevice device);

static int32_t s_allocateCommandBuffer(VkCommandBuffer *commandBuffer, VkDevice device, VkCommandPool commandPool);

static int32_t s_createSyncObjects(struct vGraph_syncObjects *syncObjects, VkDevice device);
static void s_deleteSyncObjects(struct vGraph_syncObjects *syncObjects, VkDevice device);//to change

static int32_t s_recordCommandBuffer(VkCommandBuffer commandBuffer, int32_t imageIndex, VkDevice device, struct vInit_swapchainDetails *swapchainDetails, VkRenderPass renderPass, VkPipeline graphicsPipeline, struct vGraph_frameBufferDetails *frameBufferArray);


/*------------------    globals    ------------------*/

/*------------------implementations------------------*/

int32_t
vGraph_initPipeline(struct vGraph_pipeline *graphicsPipeline, struct vInit_graphicsStruct *graphicsPacket){
    if(s_createRenderPass(&(graphicsPipeline->renderPass), graphicsPacket->device, &(graphicsPacket->swapchainDetails.imageFormat))){
        fprintf(stderr, "Error: Creating renderPass\n");
        return 1;
    }
    
    if(s_createPipeline(graphicsPipeline, graphicsPacket->device, &(graphicsPacket->swapchainDetails))){
        fprintf(stderr, "Error: Creating test pipeline\n");
        return 1;
    }
    
    if(s_createFrameBuffer(&(graphicsPipeline->frameBufferArray), graphicsPacket->device, &(graphicsPacket->swapchainDetails), &(graphicsPacket->imageViewArray), graphicsPipeline->renderPass)){
        fprintf(stderr, "Error: Creating frame buffer\n");
        return 1;
    }
    if(s_createCommandPool(&(graphicsPipeline->commandPool), &(graphicsPacket->queueIndices), graphicsPacket->device)){
        fprintf(stderr, "Error: Creating command pool\n");
        return 1;
    }
    if(s_allocateCommandBuffer(&(graphicsPipeline->commandBuffer), graphicsPacket->device, graphicsPipeline->commandPool)){
        fprintf(stderr, "Error: Creating command buffer\n");
        return 1;
    }
    if(s_createSyncObjects(&(graphicsPipeline->syncObjects), graphicsPacket->device)){
        fprintf(stderr, "Error: Creating command buffer\n");
        return 1;
    }
    return 0;
}

void
vGraph_destroyPipeline(struct vGraph_pipeline *graphicsPipeline, struct vInit_graphicsStruct *graphicsPacket){
    s_deleteSyncObjects(&(graphicsPipeline->syncObjects), graphicsPacket->device);
    s_deleteCommandPool(&(graphicsPipeline->commandPool), graphicsPacket->device);
    s_deleteFrameBuffer(&(graphicsPipeline->frameBufferArray), graphicsPacket->device);
    s_deletePipeline(graphicsPipeline, graphicsPacket->device);
    s_deleteRenderPass(&(graphicsPipeline->renderPass), graphicsPacket->device);
}

void
vGraph_waitForIdle(struct vInit_graphicsStruct *graphicsPacket){
    vkDeviceWaitIdle(graphicsPacket->device);
}

int32_t
vGraph_drawFrame(struct vInit_graphicsStruct *graphicsPacket, struct vGraph_pipeline *graphicsPipeline){
    
    vkWaitForFences(graphicsPacket->device, 1, &(graphicsPipeline->syncObjects.inFlightFence), VK_TRUE, UINT64_MAX);
    
    vkResetFences(graphicsPacket->device, 1, &(graphicsPipeline->syncObjects.inFlightFence));

    uint32_t imageIndex;
    vkAcquireNextImageKHR(graphicsPacket->device, graphicsPacket->swapchain, UINT64_MAX, graphicsPipeline->syncObjects.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
    
    vkResetCommandBuffer(graphicsPipeline->commandBuffer, 0);
    s_recordCommandBuffer(graphicsPipeline->commandBuffer, imageIndex, graphicsPacket->device, &(graphicsPacket->swapchainDetails), graphicsPipeline->renderPass, graphicsPipeline->graphicsPipeline, &(graphicsPipeline->frameBufferArray));
    
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {graphicsPipeline->syncObjects.imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &(graphicsPipeline->commandBuffer);
    
    VkSemaphore signalSemaphores[] = {graphicsPipeline->syncObjects.renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    
    if (vkQueueSubmit(graphicsPacket->queueHandles.graphicsQueue, 1, &submitInfo, graphicsPipeline->syncObjects.inFlightFence) != VK_SUCCESS) {
        fprintf(stderr, "Error:t failed to submit draw command buffer!\n");
        return 1;
    }
    
    
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {graphicsPacket->swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = NULL; // Optional

    vkQueuePresentKHR(graphicsPacket->queueHandles.presentQueue, &presentInfo);

    return 0;
}

static
int32_t
s_createPipeline(struct vGraph_pipeline *graphicsPipeline, VkDevice device, struct vInit_swapchainDetails *swapchainDetails){
    printf("Creating Pipeline\n");
    if(s_createShaderModule(&(graphicsPipeline->vertShaderModule), "res/shaders/shader.vert.spv", device)){
        return 1;
    };
    if(s_createShaderModule(&(graphicsPipeline->fragShaderModule), "res/shaders/shader.frag.spv", device)){
        return 1;
    };
    
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
    viewport.width = (float)swapchainDetails->extent.width;
    viewport.height = (float)swapchainDetails->extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    
    VkRect2D scissor = {};
    scissor.offset = (VkOffset2D){0, 0};
    scissor.extent = swapchainDetails->extent;
    
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
    *(colorBlending.blendConstants+0) = 0.0f; // Optional
    *(colorBlending.blendConstants+1) = 0.0f; // Optional
    *(colorBlending.blendConstants+2) = 0.0f; // Optional
    *(colorBlending.blendConstants+3) = 0.0f; // Optional
    
    if(s_createPipelineLayout(&(graphicsPipeline->layout), device)){
        return 1;
    }
    
    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
    pipelineInfo.pViewportState = &viewportStateCreateInfo;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = NULL; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
    
    pipelineInfo.layout = graphicsPipeline->layout;
    pipelineInfo.renderPass = graphicsPipeline->renderPass;
    pipelineInfo.subpass = 0;
    
    if(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &(graphicsPipeline->graphicsPipeline)) != VK_SUCCESS){
        fprintf(stderr, "Error: Error creating pipeline");
        return 1;
    }
    
    return 0;
}

static
void
s_deletePipeline(struct vGraph_pipeline *graphicsPipeline, VkDevice device){
    printf("deleting the pipeline\n");
    
    vkDestroyPipeline(device, graphicsPipeline->graphicsPipeline, NULL);
    graphicsPipeline->graphicsPipeline = NULL;
    
    s_deletePipelineLayout(&(graphicsPipeline->layout), device);
    graphicsPipeline->layout = NULL;
    
    s_deleteShaderModule(&(graphicsPipeline->vertShaderModule), device);
    graphicsPipeline->vertShaderModule = NULL;
    
    s_deleteShaderModule(&(graphicsPipeline->fragShaderModule), device);
    graphicsPipeline->fragShaderModule = NULL;
    
}

static
int32_t
s_createPipelineLayout(VkPipelineLayout *pipelineLayout, VkDevice device){
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 0; // Optional
    pipelineLayoutCreateInfo.pSetLayouts = NULL; // Optional
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutCreateInfo.pPushConstantRanges = NULL; // Optional
    
    if(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL, pipelineLayout) != VK_SUCCESS) {
        fprintf(stderr, "Error: Creating pipelineLayout\n");
        return 1;
    }
    return 0;
}

void
s_deletePipelineLayout(VkPipelineLayout *pipelineLayout, VkDevice device){
    vkDestroyPipelineLayout(device, *pipelineLayout, NULL);
    *pipelineLayout=NULL;
}

static
int32_t
s_createShaderModule(VkShaderModule *shaderModule, const char *shaderFileName, VkDevice device){
    struct utils_file shaderSPVFile;
    if(utils_openFile(&shaderSPVFile, shaderFileName)){
        return 1;
    }
    VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = shaderSPVFile.size;
    shaderModuleCreateInfo.pCode = (uint32_t*)(shaderSPVFile.content);
    if(vkCreateShaderModule(device, &shaderModuleCreateInfo, NULL, shaderModule)){
        fprintf(stderr, "Error: Creating shader module %s\n", shaderFileName);
        return 1;
    }
    if(utils_closeFile(&shaderSPVFile)){
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

static 
int32_t 
s_createRenderPass(VkRenderPass *renderPass, VkDevice device, const VkFormat *const swapchainImageFormat){
    VkAttachmentDescription colorAttachmentDescription = {};
    colorAttachmentDescription.format = *swapchainImageFormat;
    colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    
    colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    
    colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    
    colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    
    VkAttachmentReference colorAttachmentReference = {};
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentReference;
    
    VkSubpassDependency subpassDependency = {};
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;    
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


    
    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &colorAttachmentDescription;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpass;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.pDependencies = &subpassDependency;

    
    if(vkCreateRenderPass(device, &renderPassCreateInfo, NULL, renderPass) != VK_SUCCESS) {
        fprintf(stderr, "Error: Failed to create render pass!\n");
        return 1;
    }
    return 0;
}


static 
void
s_deleteRenderPass(VkRenderPass *renderPass, VkDevice device){
    vkDestroyRenderPass(device, *renderPass, NULL);
    *renderPass = NULL;
}

static
int32_t
s_createFrameBuffer(struct vGraph_frameBufferDetails *frameBufferArray, VkDevice device, struct vInit_swapchainDetails *swapchainDetails, struct vInit_imageViewDetails *imageViewArray, VkRenderPass renderPass){
    frameBufferArray->count = imageViewArray->count;
    frameBufferArray->frameBuffers = malloc(frameBufferArray->count*sizeof(VkFramebuffer));
    if(!frameBufferArray->frameBuffers){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    for(int32_t iter=0; iter<imageViewArray->count; iter++){
        //printf("frame buffer Creation %d\n", imageViewArray->count);
        VkImageView attachments[] = {*((imageViewArray->imageViews)+iter)};
        
        VkFramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = renderPass;
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.pAttachments = attachments;
        framebufferCreateInfo.width = swapchainDetails->extent.width;
        framebufferCreateInfo.height = swapchainDetails->extent.height;
        framebufferCreateInfo.layers = 1;
        
        if((vkCreateFramebuffer(device, &framebufferCreateInfo, NULL, (frameBufferArray->frameBuffers)+iter)) != VK_SUCCESS){
            fprintf(stderr, "Error: Creating %d frameBuffer\n", iter);
            return 1;
        }
    }
    
    return 0;
}

static
void 
s_deleteFrameBuffer(struct vGraph_frameBufferDetails *frameBufferArray, VkDevice device){
    for(int32_t iter=0; iter<frameBufferArray->count; iter++){
        vkDestroyFramebuffer(device, *(frameBufferArray->frameBuffers+iter), NULL);
    }
    free(frameBufferArray->frameBuffers);
    frameBufferArray->frameBuffers = NULL;
    frameBufferArray->count = 0;
}

static
int32_t
s_createCommandPool(VkCommandPool *commandPool, struct vInit_queueIndices *queueIndices, VkDevice device){
    //tochange index;
    
    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolCreateInfo.queueFamilyIndex = queueIndices->presentQueue;
    
    if(vkCreateCommandPool(device, &commandPoolCreateInfo, NULL, commandPool) != VK_SUCCESS){
        fprintf(stderr, "Error: failed to create command pool\n");
        return 1;
    }
    
    return 0;
}

static
void
s_deleteCommandPool(VkCommandPool *commandPool, VkDevice device){
    vkDestroyCommandPool(device, *commandPool, NULL);
    *commandPool = NULL;
}

static
int32_t
s_allocateCommandBuffer(VkCommandBuffer *commandBuffer, VkDevice device, VkCommandPool commandPool){
    
    VkCommandBufferAllocateInfo commandBufferCreateInfo = {};
    commandBufferCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferCreateInfo.commandPool = commandPool;
    commandBufferCreateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferCreateInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(device, &commandBufferCreateInfo, commandBuffer) != VK_SUCCESS) {
        fprintf(stderr, "Error: Failed to allocate command buffers!");
        return 1;
    }
    return 0;
}

static
int32_t
s_createSyncObjects(struct vGraph_syncObjects *syncObjects, VkDevice device){
    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    
    
    if(vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &(syncObjects->imageAvailableSemaphore)) != VK_SUCCESS){
        fprintf(stderr, "Error: error creating imageAvailableSemaphore\n");
        return 1;
    }
    
    if(vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &(syncObjects->renderFinishedSemaphore)) != VK_SUCCESS){
        fprintf(stderr, "Error: error creating renderFinishedSemaphore\n");
        return 1;
    }
    
    if(vkCreateFence(device, &fenceCreateInfo, NULL, &(syncObjects->inFlightFence)) != VK_SUCCESS){
        fprintf(stderr, "Error: error creating inFlightFence\n");
        return 1;
    }
    
    return 0;
}

static
void
s_deleteSyncObjects(struct vGraph_syncObjects *syncObjects, VkDevice device){
    vkDestroySemaphore(device, syncObjects->imageAvailableSemaphore, NULL);
    vkDestroySemaphore(device, syncObjects->renderFinishedSemaphore, NULL);
    vkDestroyFence(device, syncObjects->inFlightFence, NULL);
    
    syncObjects->imageAvailableSemaphore = NULL;
    syncObjects->renderFinishedSemaphore = NULL;
    syncObjects->inFlightFence = NULL;
}

static
int32_t 
s_recordCommandBuffer(VkCommandBuffer commandBuffer, int32_t imageIndex, VkDevice device, struct vInit_swapchainDetails *swapchainDetails, VkRenderPass renderPass, VkPipeline graphicsPipeline, struct vGraph_frameBufferDetails *frameBufferArray){
    
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        fprintf(stderr, "failed to begin recording command buffer!");
        return 1;
    }
    
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = frameBufferArray->frameBuffers[imageIndex];
    
    renderPassInfo.renderArea.offset = (VkOffset2D){0, 0};
    renderPassInfo.renderArea.extent = swapchainDetails->extent;

    
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)(swapchainDetails->extent.width);
    viewport.height = (float)(swapchainDetails->extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor = {};
    scissor.offset = (VkOffset2D){0, 0};
    scissor.extent = swapchainDetails->extent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    
    vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    
    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        fprintf(stderr, "Error: failed to record command buffer\n");
        return 1;
    }
    
    return 0;
}

