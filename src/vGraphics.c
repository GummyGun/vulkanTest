#include "vInit.h"
#include "vGraphics.h"
#include "utils.h"

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include <cglm/cglm.h>

/*
#define STB_IMAGE_IMPLEMENTATION
#include <stb.h>
#include <stb_image.h>
*/

/*------------------  defines ------------------*/

#define FRAMES_IN_FLIGHT 2

/*------------------  enums   ------------------*/

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

static int32_t s_createPipelineLayout(VkPipelineLayout *pipelineLayout, VkDevice device, VkDescriptorSetLayout *descriptorSetLayout);
static void s_deletePipelineLayout(VkPipelineLayout *pipelineLayout, VkDevice device);

static int32_t s_createRenderPass(VkRenderPass *renderPass, VkDevice device, const VkFormat *const swapchainImageFormat);
static void s_deleteRenderPass(VkRenderPass *renderPass, VkDevice device);

static int32_t s_createDescriptorSetLayout(VkDescriptorSetLayout *descriptorSetLayout, VkDevice device);
static void s_deleteDescriptorSetLayout(VkDescriptorSetLayout *descriptorSetLayout, VkDevice device);

static int32_t s_createFrameBuffer(struct vGraph_frameBufferDetails *frameBufferArray, VkDevice device, struct vInit_swapchainDetails *swapchainDetails, struct vInit_imageViewDetails *imageViewArray, VkRenderPass renderPass);
static void s_deleteFrameBuffer(struct vGraph_frameBufferDetails *frameBufferArray, VkDevice device);

static int32_t s_createCommandPool(VkCommandPool *commandPool, struct vInit_queueIndices *queueIndices, VkDevice device);//to change
static void s_deleteCommandPool(VkCommandPool *commandPool, VkDevice device);

static int32_t s_createBuffer(VkBuffer *buffer, VkDeviceMemory *bufferMemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memProperties, const VkPhysicalDeviceMemoryProperties const *PDMemProperties, VkDevice device);
static void s_deleteBuffer(VkBuffer *buffer, VkDeviceMemory *bufferMemory, VkDevice device);
static int32_t s_findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags memoryProperties, const VkPhysicalDeviceMemoryProperties *const PDMemProperties);

static void s_copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize bufferSize, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool);

static int32_t s_createVertexBuffer(VkBuffer *vertexBuffer, VkDeviceMemory *vertexBufferMemory, const VkPhysicalDeviceMemoryProperties const *PDMemProperties, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool);
static void s_deleteVertexBuffer(VkBuffer *vertexBuffer, VkDeviceMemory *vertexBufferMemory, VkDevice device);
    
static int32_t s_createIndexBuffer(VkBuffer *indexBuffer, VkDeviceMemory *indexBufferMemory, const VkPhysicalDeviceMemoryProperties const *PDMemProperties, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool);
static void s_deleteIndexBuffer(VkBuffer *indexBuffer, VkDeviceMemory *indexBufferMemory, VkDevice device);

static int32_t s_createUniformBuffers(struct utils_pointerArray *uniformBuffers, struct utils_pointerArray *uniformBuffersMemory, struct utils_pointerArray *uniformBuffersMap, const VkPhysicalDeviceMemoryProperties const *PDMemProperties, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool);
static void s_deleteUnionBuffers(struct utils_pointerArray *uniformBuffers, struct utils_pointerArray *uniformBuffersMemory, struct utils_pointerArray *uniformBuffersMap, VkDevice device);
    
static int32_t s_allocateCommandBuffer(struct vGraph_commandBufferDetails *commandBufferArray, VkDevice device, VkCommandPool commandPool);

static int32_t s_createSyncObjects(struct vGraph_syncObjects *syncObjects, VkDevice device);
static void s_deleteSyncObjects(struct vGraph_syncObjects *syncObjects, VkDevice device);//to change

static int32_t s_recordCommandBuffer(VkCommandBuffer commandBuffer, int32_t currentFrame, VkDevice device, struct vInit_swapchainDetails *swapchainDetails, VkRenderPass renderPass, VkPipeline graphicsPipeline, struct vGraph_frameBufferDetails *frameBufferArray, VkBuffer vertexBuffer, VkBuffer indexBuffer);

static void s_updateUniformBuffer(int32_t currentFrame, VkExtent2D *extent, struct utils_pointerArray *uniformBuffersMap);

/*------------------    globals    ------------------*/

#ifdef pent
//triangle vertex
static const struct vGraph_simpleVertex simpleVertexArray[] = 
{
/*0*/   {{0.0f, 0.0f},{1.0f, 1.0f, 1.0f}},
/*1*/   {{0.0f, -0.8f},{1.0f, 0.0f, 0.0f}},
/*2*/   {{0.19857324855468f, -0.18885438199982f},{1.0f, 1.0f, 0.0f}}, 
/*3*/   {{0.841169779391f, -0.188854382f},{0.0f, 1.0f, 0.0f}}, 
/*4*/   {{0.32129826541796f, 0.18885438199982f},{0.0f, 1.0f, 1.0f}}, 
/*5*/   {{0.519871513973f, 0.8f},{0.0f, 0.0f, 1.0f}}, 
/*6*/   {{0.0f, 0.42229123600033f},{1.0f, 0.0f, 1.0f}}, 
/*7*/   {{-0.519871513973f, 0.8f},{1.0f, 0.0f, 0.0f}},
/*8*/   {{-0.32129826541796f, 0.18885438199982f},{1.0f, 1.0f, 0.0f}}, 

/*0*/   {{-0.841169779391f, -0.188854382f},{0.0f, 1.0f, 0.0f}}, 
/*a*/   {{-0.19857324855468f, -0.18885438199982f},{0.0f, 1.0f, 1.0f}}, 
    
};
/*
             ..---1---..
        .--""'   / \   '""--.
      ."        |   |        ".
    ."          |   |          ".
   /           /     \           \
  /           |       |           \
 9------------a-------2------------3
.'`--.       /         \       .--' .
:     ".    |           |    ."     ;
|       "-. |     0     | .-"       |
|          8-.         .-4          |
"         |   ".     ."   |         "
 \       /      "-6-"      \       /
 ".     |      .-' '-.      |     ."
  \     |    ."       ".    |     /
   \   /  .-"           "-.  \   /
    ".|.-"                 "-.|."
      7.                     .5
        "--..           ..--"
            '""-------""'
*/
static const uint16_t simpleIndexArray[] =
{
    1,3,5, 1,5,7, 1,7,9, 1,4,8, 0xa,3,6, 2,5,8, 0xa,4,7, 9,2,6, 0xa,2,0, 2,4,0, 0,4,6, 0,6,8, 0xa,0,8
};

/* pentagrama mas estrella
    1,3,5, 1,5,7, 1,7,9, 1,2,0xa, 2,3,4, 4,5,6, 8,6,7, 9,0xa,8, 0xa,2,0, 2,4,0, 4,6,0, 0,6,8, 0xa,0,8
*/

/* start 
    1,2,0xa, 2,3,4, 4,5,6, 8,6,7, 9,0xa,8, 0xa,2,0, 2,4,0, 4,6,0, 0,6,8, 0xa,0,8
*/

/* pentagram
    1,3,5, 1,5,7, 1,7,9
*/

/* negative star
    1,3,2, 3,5,4, 6,5,7, 9,8,7, 1,0xa,9
*/

#endif

static const struct vGraph_simpleVertex simpleVertexArray[] = {
    {{-0.15f, -0.9f}, {1.0f ,1.0f ,1.0f}},
    {{0.15f, -0.9f}, {1.0f ,1.0f ,1.0f}},
    {{0.15f, -0.6f}, {1.0f ,1.0f ,1.0f}},
    {{-0.15f, -0.6f}, {1.0f ,1.0f ,1.0f}},
    
    {{-0.1f, -0.5f}, {1.0f ,1.0f ,1.0f}},
    {{0.1f, -0.5f}, {1.0f ,1.0f ,1.0f}},
    {{0.1f, 0.1f}, {1.0f ,1.0f ,1.0f}},
    {{-0.1f, 0.1f}, {1.0f ,1.0f ,1.0f}},
    
    {{0.15f, -0.4f}, {1.0f ,1.0f ,1.0f}},
    {{0.8f, -0.4f}, {1.0f ,1.0f ,1.0f}},
    {{0.8f, -0.3f}, {1.0f ,1.0f ,1.0f}},
    {{0.15f, -0.3f}, {1.0f ,1.0f ,1.0f}},
    
    {{-0.8f, -0.4f}, {1.0f ,1.0f ,1.0f}},
    {{-0.15f, -0.4f}, {1.0f ,1.0f ,1.0f}},
    {{-0.15f, -0.3f}, {1.0f ,1.0f ,1.0f}},
    {{-0.8f, -0.3f}, {1.0f ,1.0f ,1.0f}},
    
    
    {{0.1f, 0.15f}, {1.0f ,1.0f ,1.0f}},
    {{0.2f, 0.15f}, {1.0f ,1.0f ,1.0f}},
    {{0.2f, 0.9f}, {1.0f ,1.0f ,1.0f}},
    {{0.1f, 0.9f}, {1.0f ,1.0f ,1.0f}},
    
    {{-0.2f, 0.15f}, {1.0f ,1.0f ,1.0f}},
    {{-0.1f, 0.15f}, {1.0f ,1.0f ,1.0f}},
    {{-0.1f, 0.9f}, {1.0f ,1.0f ,1.0f}},
    {{-0.2f, 0.9f}, {1.0f ,1.0f ,1.0f}},
};

static const uint16_t simpleIndexArray[] = {
    0,1,2,0,2,3, 4,5,6,4,6,7, 8,9,0xa,8,0xa,0xb, 0xc,0xd,0xe,0xc,0xe,0xf, 0x10,0x11,0x12,0x10,0x12,0x13, 0x14,0x15,0x16,0x14,0x16,0x17
};

static const int32_t simpleVertexArrayCount = sizeof(simpleVertexArray)/sizeof(struct vGraph_simpleVertex);
static const int32_t simpleVertexSize = sizeof(struct vGraph_simpleVertex);

static const VkVertexInputBindingDescription vertexInputBindingDescription = {
    .binding = 0,
    .stride = sizeof(struct vGraph_simpleVertex),
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
};

static const VkVertexInputAttributeDescription vertexInputAttributeDescription[] = {
    {.binding=0, .location=0, .format=VK_FORMAT_R32G32_SFLOAT, offsetof(struct vGraph_simpleVertex, position)},
    {.binding=0, .location=1, .format=VK_FORMAT_R32G32B32_SFLOAT, offsetof(struct vGraph_simpleVertex, color)}
};
static const int32_t vertexInputAttributeDescriptionCount = sizeof(vertexInputAttributeDescription)/sizeof(struct VkVertexInputAttributeDescription);


static const int32_t simpleIndexArrayCount = sizeof(simpleIndexArray)/sizeof(int16_t);
static const int32_t simpleIndexSize = sizeof(int16_t);


/*------------------implementations------------------*/

/* ================================= extern methods ================================== */
/*                                                                                     */
/*                                                                                     */
/*                _                     __                  _   _                      */
/*               | |                   / _|                | | (_)                     */
/*       _____  _| |_ ___ _ __ _ __   | |_ _   _ _ __   ___| |_ _  ___  _ __  ___      */
/*      / _ \ \/ / __/ _ \ '__| '_ \  |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|     */
/*     |  __/>  <| ||  __/ |  | | | | | | | |_| | | | | (__| |_| | (_) | | | \__ \     */
/*      \___/_/\_\\__\___|_|  |_| |_| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/     */
/*                                                                                     */
/*                                                                                     */
/*                                                                                     */
/* ================================= extern methods ================================== */

int32_t
vGraph_initPipeline(struct vGraph_pipeline *graphicsPipeline, struct vInit_graphicsStruct *graphicsPacket){
    if(s_createRenderPass(&(graphicsPipeline->renderPass), graphicsPacket->device, &(graphicsPacket->swapchainDetails.imageFormat))){
        fprintf(stderr, "Error: Creating renderPass\n");
        return 1;
    }
    if(s_createDescriptorSetLayout(&(graphicsPipeline->descriptorSetLayout), graphicsPacket->device)){
        fprintf(stderr, "Error: Creating descriptor set layout\n");
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
    if(s_createVertexBuffer(&(graphicsPipeline->vertexBuffer), &(graphicsPipeline->vertexBufferMemory), &(graphicsPacket->PDMemProperties), graphicsPacket->device, graphicsPacket->queueHandles.graphicsQueue, graphicsPipeline->commandPool)){
        fprintf(stderr, "Error: Creating vertex buffer\n");
        return 1;
    }
    if(s_createIndexBuffer(&(graphicsPipeline->indexBuffer), &(graphicsPipeline->indexBufferMemory), &(graphicsPacket->PDMemProperties), graphicsPacket->device, graphicsPacket->queueHandles.graphicsQueue, graphicsPipeline->commandPool)){
        fprintf(stderr, "Error: Creating index buffer\n");
        return 1;
    }
    if(s_createUniformBuffers(&(graphicsPipeline->uniformBuffers), &(graphicsPipeline->uniformBuffersMemory), &(graphicsPipeline->uniformBuffersMap), &(graphicsPacket->PDMemProperties), graphicsPacket->device, graphicsPacket->queueHandles.graphicsQueue, graphicsPipeline->commandPool)){
        fprintf(stderr, "Error: Creating uniform buffer\n");
        return 1;
    }
    if(s_allocateCommandBuffer(&(graphicsPipeline->commandBufferArray), graphicsPacket->device, graphicsPipeline->commandPool)){
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
    s_deleteVertexBuffer(&(graphicsPipeline->vertexBuffer), &(graphicsPipeline->vertexBufferMemory), graphicsPacket->device);
    s_deleteIndexBuffer(&(graphicsPipeline->indexBuffer), &(graphicsPipeline->indexBufferMemory), graphicsPacket->device);
    s_deleteUnionBuffers(&(graphicsPipeline->uniformBuffers), &(graphicsPipeline->uniformBuffersMemory), &(graphicsPipeline->uniformBuffersMap), graphicsPacket->device);
    s_deleteFrameBuffer(&(graphicsPipeline->frameBufferArray), graphicsPacket->device);
    s_deletePipeline(graphicsPipeline, graphicsPacket->device);
    s_deleteDescriptorSetLayout(&(graphicsPipeline->descriptorSetLayout), graphicsPacket->device);
    s_deleteRenderPass(&(graphicsPipeline->renderPass), graphicsPacket->device);
}

void
vGraph_waitForIdle(struct vInit_graphicsStruct *graphicsPacket){
    vkDeviceWaitIdle(graphicsPacket->device);
}

int32_t
vGraph_drawFrame(struct vInit_graphicsStruct *graphicsPacket, struct vGraph_pipeline *graphicsPipeline){
    
    vkWaitForFences(graphicsPacket->device, 1, (graphicsPipeline->syncObjects.inFlightFenceArray.fences)+graphicsPipeline->currentFrame, VK_TRUE, UINT64_MAX);
    
    vkResetFences(graphicsPacket->device, 1, (graphicsPipeline->syncObjects.inFlightFenceArray.fences)+graphicsPipeline->currentFrame);

    uint32_t currentFrame;
    vkAcquireNextImageKHR(graphicsPacket->device, graphicsPacket->swapchain, UINT64_MAX, *((graphicsPipeline->syncObjects.imageAvailableSemaphoreArray.semaphores)+graphicsPipeline->currentFrame), VK_NULL_HANDLE, &currentFrame);
    
    vkResetCommandBuffer(*((graphicsPipeline->commandBufferArray.commandBuffers)+graphicsPipeline->currentFrame), 0);
    s_recordCommandBuffer(*((graphicsPipeline->commandBufferArray.commandBuffers)+graphicsPipeline->currentFrame), currentFrame, graphicsPacket->device, &(graphicsPacket->swapchainDetails), graphicsPipeline->renderPass, graphicsPipeline->graphicsPipeline, &(graphicsPipeline->frameBufferArray), graphicsPipeline->vertexBuffer, graphicsPipeline->indexBuffer);
    
    s_updateUniformBuffer(graphicsPipeline->currentFrame, &(graphicsPacket->swapchainDetails.extent), &(graphicsPipeline->uniformBuffersMap));
    
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {*((graphicsPipeline->syncObjects.imageAvailableSemaphoreArray.semaphores)+graphicsPipeline->currentFrame)};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = ((graphicsPipeline->commandBufferArray.commandBuffers)+graphicsPipeline->currentFrame);
    
    VkSemaphore signalSemaphores[] = {*((graphicsPipeline->syncObjects.renderFinishedSemaphoreArray.semaphores)+graphicsPipeline->currentFrame)};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    
    if (vkQueueSubmit(graphicsPacket->queueHandles.graphicsQueue, 1, &submitInfo, *((graphicsPipeline->syncObjects.inFlightFenceArray.fences)+graphicsPipeline->currentFrame)) != VK_SUCCESS) {
        fprintf(stderr, "Error:T failed to submit draw command buffer!\n");
        return 1;
    }
    
    
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {graphicsPacket->swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &currentFrame;

    presentInfo.pResults = NULL; // Optional

    vkQueuePresentKHR(graphicsPacket->queueHandles.presentQueue, &presentInfo);
    
    graphicsPipeline->currentFrame = (graphicsPipeline->currentFrame+1)%FRAMES_IN_FLIGHT;
    graphicsPipeline->totalFrames++;

    return 0;
}

/* =============================== static methods =============================== */
/*                                                                                */
/*                                                                                */
/*          _        _   _         __                  _   _                      */
/*         | |      | | (_)       / _|                | | (_)                     */
/*      ___| |_ __ _| |_ _  ___  | |_ _   _ _ __   ___| |_ _  ___  _ __  ___      */
/*     / __| __/ _` | __| |/ __| |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|     */
/*     \__ \ || (_| | |_| | (__  | | | |_| | | | | (__| |_| | (_) | | | \__ \     */
/*     |___/\__\__,_|\__|_|\___| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/     */
/*                                                                                */
/*                                                                                */
/*                                                                                */
/* =============================== static methods =============================== */

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
    
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &vertexInputBindingDescription; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = vertexInputAttributeDescriptionCount;
    vertexInputInfo.pVertexAttributeDescriptions = vertexInputAttributeDescription; // Optional
    
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
    
    printf("extent size %d %d\n", swapchainDetails->extent.width, swapchainDetails->extent.height);
    
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
    
    if(s_createPipelineLayout(&(graphicsPipeline->layout), device, &(graphicsPipeline->descriptorSetLayout))){
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
s_createPipelineLayout(VkPipelineLayout *pipelineLayout, VkDevice device, VkDescriptorSetLayout *descriptorSetLayout){
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 1; 
    pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayout; 
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
s_createDescriptorSetLayout(VkDescriptorSetLayout *descriptorSetLayout, VkDevice device){
    VkDescriptorSetLayoutBinding UBOLayoutBuffer = {};
    UBOLayoutBuffer.binding = 0;
    UBOLayoutBuffer.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    UBOLayoutBuffer.descriptorCount = 1;
    UBOLayoutBuffer.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    
    UBOLayoutBuffer.pImmutableSamplers = NULL;
    
    //VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.bindingCount = 1;
    descriptorSetLayoutCreateInfo.pBindings = &UBOLayoutBuffer;
    
    if(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, NULL, descriptorSetLayout) != VK_SUCCESS){
        fprintf(stderr, "Error: Creating descriptor set Layout\n");
        return 1;
    }
    
    return 0;
}

static 
void
s_deleteDescriptorSetLayout(VkDescriptorSetLayout *descriptorSetLayout, VkDevice device){
    vkDestroyDescriptorSetLayout(device, *descriptorSetLayout, NULL);
    *descriptorSetLayout = NULL;
    
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
        fprintf(stderr, "Error: Failed to create command pool\n");
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
s_createBuffer(VkBuffer *buffer, VkDeviceMemory *bufferMemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memProperties, const VkPhysicalDeviceMemoryProperties const *PDMemProperties, VkDevice device){
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = usage;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    //printf("vertexNodeSize %d\nvertexCount %d\n", simpleVertexSize, simpleVertexArrayCount);
    if(vkCreateBuffer(device, &bufferCreateInfo, NULL, buffer) != VK_SUCCESS){
        fprintf(stderr, "Error: Error creating vertex buffer\n");
        return 1;
    }
    VkMemoryRequirements memoryRequirements = {};
    vkGetBufferMemoryRequirements(device, *buffer, &memoryRequirements);
    
    VkMemoryAllocateInfo memoryAllocationInfo = {};
    memoryAllocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocationInfo.allocationSize = memoryRequirements.size;
    memoryAllocationInfo.memoryTypeIndex = s_findMemoryType(memoryRequirements.memoryTypeBits, memProperties, PDMemProperties);
    
    if(vkAllocateMemory(device, &memoryAllocationInfo, NULL, bufferMemory) != VK_SUCCESS){
        fprintf(stderr, "Error: Error allocating vertex buffer memory\n");
        return 1;
    }
    
    vkBindBufferMemory(device, *buffer, *bufferMemory, 0);
    
    return 0;
}

static 
void 
s_deleteBuffer(VkBuffer *vertexBuffer, VkDeviceMemory *vertexBufferMemory, VkDevice device){
    vkFreeMemory(device, *vertexBufferMemory, NULL);
    *vertexBufferMemory = NULL;
    vkDestroyBuffer(device, *vertexBuffer, NULL);
    *vertexBuffer = NULL;
}

static 
int32_t
s_findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags memoryProperties, const VkPhysicalDeviceMemoryProperties *const PDMemProperties){
    for(int32_t iter=0; iter<PDMemProperties->memoryTypeCount; iter++){
        if((typeFilter & (1<<iter)) && (((PDMemProperties->memoryTypes)+iter)->propertyFlags & memoryProperties) == memoryProperties){
            return iter;
        }
    }
    return -1;
}

static
void
s_copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize bufferSize, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool){
    VkCommandBufferAllocateInfo bufferAllocateInfo = {};
    bufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    bufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    bufferAllocateInfo.commandPool = commandPool;
    bufferAllocateInfo.commandBufferCount = 1;
    
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &bufferAllocateInfo, &commandBuffer);
    
    VkCommandBufferBeginInfo cmdBufferBeginInfo  = {};
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    
    vkBeginCommandBuffer(commandBuffer, &cmdBufferBeginInfo);
    
    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = bufferSize;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    
    vkEndCommandBuffer(commandBuffer);
    
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);
    
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

static
int32_t
s_createVertexBuffer(VkBuffer *vertexBuffer, VkDeviceMemory *vertexBufferMemory, const VkPhysicalDeviceMemoryProperties const *PDMemProperties, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool){
    
    VkDeviceSize bufferSize = simpleVertexSize*simpleVertexArrayCount;
    
    VkBuffer stagingBuffer = NULL;
    VkDeviceMemory stagingBufferMemory = NULL;
    
    if(s_createBuffer(&stagingBuffer, &stagingBufferMemory, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, PDMemProperties, device)){
        fprintf(stderr, "Error: Creating Vertex Buffer\n");
        return 1;
    }
    
    void *data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, simpleVertexArray, bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);
    
    if(s_createBuffer(vertexBuffer, vertexBufferMemory, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, PDMemProperties, device)){
        fprintf(stderr, "Error: Creating Vertex Buffer\n");
        return 1;
    }
    
    s_copyBuffer(stagingBuffer, *vertexBuffer, bufferSize, device, graphicsQueue, commandPool);
    
    s_deleteBuffer(&stagingBuffer, &stagingBufferMemory, device);
    
    return 0;
}

static 
void 
s_deleteVertexBuffer(VkBuffer *vertexBuffer, VkDeviceMemory *vertexBufferMemory, VkDevice device){
    s_deleteBuffer(vertexBuffer, vertexBufferMemory, device);
}

static
int32_t 
s_createIndexBuffer(VkBuffer *indexBuffer, VkDeviceMemory *indexBufferMemory, const VkPhysicalDeviceMemoryProperties const *PDMemProperties, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool){
    
    VkDeviceSize bufferSize = simpleIndexSize* simpleIndexArrayCount;
    
    VkBuffer stagingBuffer = NULL;
    VkDeviceMemory stagingBufferMemory = NULL;
    
    if(s_createBuffer(&stagingBuffer, &stagingBufferMemory, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, PDMemProperties, device)){
        fprintf(stderr, "Error: Creating Vertex Buffer\n");
        return 1;
    }
    
    void *data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, simpleIndexArray, bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);
    
    if(s_createBuffer(indexBuffer, indexBufferMemory, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, PDMemProperties, device)){
        fprintf(stderr, "Error: Creating Vertex Buffer\n");
        return 1;
    }
    
    s_copyBuffer(stagingBuffer, *indexBuffer, bufferSize, device, graphicsQueue, commandPool);
    
    s_deleteBuffer(&stagingBuffer, &stagingBufferMemory, device);
    
    return 0;
}

static 
void 
s_deleteIndexBuffer(VkBuffer *indexBuffer, VkDeviceMemory *indexBufferMemory, VkDevice device){
    s_deleteBuffer(indexBuffer, indexBufferMemory, device);
    
}

static 
int32_t
s_createUniformBuffers(struct utils_pointerArray *uniformBuffers, struct utils_pointerArray *uniformBuffersMemory, struct utils_pointerArray *uniformBuffersMap, const VkPhysicalDeviceMemoryProperties const *PDMemProperties, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool){
    //printf("-------------------------------------\n");
    int32_t uniformBufferSize = sizeof(struct vGraph_uniformBufferObject);
    utils_createPArray(uniformBuffers, FRAMES_IN_FLIGHT);
    utils_createPArray(uniformBuffersMemory, FRAMES_IN_FLIGHT);
    utils_createPArray(uniformBuffersMap, FRAMES_IN_FLIGHT);
    printf("uniform buffer size %d bytes\n", uniformBufferSize);
    
    for(int32_t iter=0; iter<2; iter++){
        
        if(s_createBuffer(((VkBuffer*)uniformBuffers->data)+iter, ((VkDeviceMemory*)uniformBuffersMemory->data)+iter, uniformBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, PDMemProperties, device)){
            fprintf(stderr, "Error: generating the uniform buffers\n");
            return 1;
        }
        
        vkMapMemory(device, *(((VkDeviceMemory*)uniformBuffersMemory->data)+iter), 0, uniformBufferSize, 0, uniformBuffersMap->data+iter);
        
    }
    return 0;
}

static
void
s_deleteUnionBuffers(struct utils_pointerArray *uniformBuffers, struct utils_pointerArray *uniformBuffersMemory, struct utils_pointerArray *uniformBuffersMap, VkDevice device){
    
    for(int32_t iter=0; iter<FRAMES_IN_FLIGHT; iter++){
        vkUnmapMemory(device, *(((VkDeviceMemory*)uniformBuffersMemory->data)+iter)); 
        s_deleteBuffer(((VkBuffer*)uniformBuffers->data)+iter, ((VkDeviceMemory*)uniformBuffersMemory->data)+iter, device);
        
    }
    utils_deletePArray(uniformBuffers);
    utils_deletePArray(uniformBuffersMemory);
    utils_deletePArray(uniformBuffersMap);
}
    
static
int32_t
s_allocateCommandBuffer(struct vGraph_commandBufferDetails *commandBufferArray, VkDevice device, VkCommandPool commandPool){
    commandBufferArray->commandBuffers=malloc(FRAMES_IN_FLIGHT*sizeof(VkCommandBuffer));
    if(!commandBufferArray->commandBuffers){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    commandBufferArray->count = FRAMES_IN_FLIGHT;
    
    
    VkCommandBufferAllocateInfo commandBufferCreateInfo = {};
    commandBufferCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferCreateInfo.commandPool = commandPool;
    commandBufferCreateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferCreateInfo.commandBufferCount = commandBufferArray->count;

    if(vkAllocateCommandBuffers(device, &commandBufferCreateInfo, commandBufferArray->commandBuffers) != VK_SUCCESS) {
        fprintf(stderr, "Error: Failed to allocate command buffers!\n");
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
    
    syncObjects->imageAvailableSemaphoreArray.semaphores = malloc(FRAMES_IN_FLIGHT*sizeof(VkSemaphore));
    syncObjects->imageAvailableSemaphoreArray.count = FRAMES_IN_FLIGHT;
    syncObjects->renderFinishedSemaphoreArray.semaphores = malloc(FRAMES_IN_FLIGHT*sizeof(VkSemaphore));
    syncObjects->renderFinishedSemaphoreArray.count = FRAMES_IN_FLIGHT;
    syncObjects->inFlightFenceArray.fences = malloc(FRAMES_IN_FLIGHT*sizeof(VkFence));
    syncObjects->inFlightFenceArray.count = FRAMES_IN_FLIGHT;
    
    if(!syncObjects->imageAvailableSemaphoreArray.semaphores || !syncObjects->renderFinishedSemaphoreArray.semaphores || !syncObjects->inFlightFenceArray.fences){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    
    
    for(int32_t iter=0; iter<FRAMES_IN_FLIGHT; iter++){
        if(vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, ((syncObjects->imageAvailableSemaphoreArray.semaphores)+iter)) != VK_SUCCESS){
            fprintf(stderr, "Error: Error creating imageAvailableSemaphore\n");
            return 1;
        }
        if(vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, ((syncObjects->renderFinishedSemaphoreArray.semaphores)+iter)) != VK_SUCCESS){
            fprintf(stderr, "Error: Error creating renderFinishedSemaphore\n");
            return 1;
        }
        if(vkCreateFence(device, &fenceCreateInfo, NULL, ((syncObjects->inFlightFenceArray.fences)+iter)) != VK_SUCCESS){
            fprintf(stderr, "Error: Error creating inFlightFence\n");
            return 1;
        }
        
    }
    
    return 0;
}

static
void
s_deleteSyncObjects(struct vGraph_syncObjects *syncObjects, VkDevice device){
    for(int32_t iter=0; iter<FRAMES_IN_FLIGHT; iter++){
        vkDestroySemaphore(device, *((syncObjects->imageAvailableSemaphoreArray.semaphores)+iter), NULL);
        vkDestroySemaphore(device, *((syncObjects->renderFinishedSemaphoreArray.semaphores)+iter), NULL);
        vkDestroyFence(device, *((syncObjects->inFlightFenceArray.fences)+iter), NULL);
    }
    
    free(syncObjects->imageAvailableSemaphoreArray.semaphores);
    free(syncObjects->renderFinishedSemaphoreArray.semaphores);
    free(syncObjects->inFlightFenceArray.fences);
    
    syncObjects->imageAvailableSemaphoreArray.semaphores = NULL;
    syncObjects->renderFinishedSemaphoreArray.semaphores = NULL;
    syncObjects->inFlightFenceArray.fences = NULL;
    
    syncObjects->imageAvailableSemaphoreArray.count = 0;
    syncObjects->renderFinishedSemaphoreArray.count = 0;
    syncObjects->inFlightFenceArray.count = 0;
    
}

static
int32_t
s_recordCommandBuffer(VkCommandBuffer commandBuffer, int32_t currentFrame, VkDevice device, struct vInit_swapchainDetails *swapchainDetails, VkRenderPass renderPass, VkPipeline graphicsPipeline, struct vGraph_frameBufferDetails *frameBufferArray, VkBuffer vertexBuffer, VkBuffer indexBuffer){
    
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        fprintf(stderr, "failed to begin recording command buffer!");
        return 1;
    }
    
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = frameBufferArray->frameBuffers[currentFrame];
    
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
    
    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize vertexOffsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, vertexOffsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    
    //printf("testestsetsetsetsetsetset- --------------------%d\n", simpleIndexArrayCount);
    vkCmdDrawIndexed(commandBuffer, simpleIndexArrayCount, 1, 0, 0, 0);
    //printf("potencial Error\n");
    
    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        fprintf(stderr, "Error: Failed to record command buffer\n");
        return 1;
    }
    
    return 0;
}

static int32_t testPrintMat4(mat4 mat);

static 
void
s_updateUniformBuffer(int32_t currentFrame, VkExtent2D *extent, struct utils_pointerArray *uniformBuffersMap){
    struct vGraph_uniformBufferObject localUniformBuffer = {};
    vec3 test = GLM_VEC3_ZERO_INIT;
    test[2] = 1.0f;
    
    glm_mat4_copy(GLM_MAT4_IDENTITY, localUniformBuffer.model);
    
    glm_rotate(localUniformBuffer.model, 2*glm_rad(45), test);
    //testPrintMat4(localUniformBuffer.model);
    
    static mat4  lookAtPreCalc = {
        {-0.707107f, -0.408248f, 0.57735f, 0.0f},
        {0.707107f, -0.408248f, 0.57735f, 0.0f},
        {0.0f, 0.816497f, 0.57735f, 0.0f},
        {0.0f, 0.0f, -3.4641f, 1.0f,}
    };
    glm_mat4_copy(lookAtPreCalc, localUniformBuffer.view);
    
    //testPrintMat4(localUniformBuffer.view);
    
    //printf("%d %d\n", extent->width, extent->height);
    glm_perspective(glm_rad(45), extent->width/(float)extent->height, 0.1f, 10.0f, localUniformBuffer.proj);
    
    localUniformBuffer.proj[1][1] *= -1;
    
    ;
    
    //testPrintMat4(localUniformBuffer.proj);
    //exit(12);
}

//test matrix print function
static
int32_t
testPrintMat4(mat4 mat){
    printf("---------------------------------------\n");
    for(int32_t iterY=0; iterY<4; iterY++){
        for(int32_t iterX=0; iterX<4; iterX++){
            printf("%f ", mat[iterY][iterX]);
        }
        printf("\n");
    }
    printf("---------------------------------------\n");
}
