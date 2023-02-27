#include "window.h"
#include "vInit.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*------------------defines------------------*/
#define VULKAN_MAX_LAYER_NAME 64

/*------------------  enums  ------------------*/

/*------------------  stucts  ------------------*/

struct vInit_swapchainSupportDetails{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    uint32_t surfaceFormatsCount;
    VkSurfaceFormatKHR *surfaceFormats;
    uint32_t presentModesCount;
    VkPresentModeKHR *presentModes;
};

/*------------------prototipes------------------*/

/* window ->instance -> surface -> physicalDevice -> queueIndices -> device -> queueHandles -> Queue -> swapchain -> swapchainDetails -> images -> imageViews */

//int32_t vInit_initVulkan(struct vInit_graphicsStruct *graphicsPacket, struct window_window *window);
//int32_t vInit_createInstance(VkInstance *instance, int32_t debugMode);
//int32_t vInit_createDebugMessenger(VkDebugUtilsMessengerEXT *debugMessenger, int32_t debugMode, VkInstance instance);
//int32_t vInit_createSurface(VkSurfaceKHR *surface, VkInstance instance, struct window_window *window);
//int32_t vInit_selectPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, VkSurfaceKHR surface);
//int32_t vInit_createDevice(VkDevice *device, struct vInit_queueIndices *queueIndices, struct vInit_queueHandles *queueHandles, VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
//int32_t vInit_createSwapchain(VkSwapchainKHR *swapchain, struct vInit_swapchainDetails *swapchainDetails, struct vInit_imageDetails *imageDetails, struct window_window *window, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, struct vInit_queueIndices *queueIndices, VkDevice device);
//int32_t vInit_createImageViews(struct vInit_imageViewDetails *imageViewArray, const struct vInit_imageDetails *imageArray, VkDevice device, const VkFormat *swapchainImageFormat);


//deletion functions

//void vInit_deleteImageViews(struct vInit_imageViewDetails *restrict imageViewArray, VkDevice device);
//void vInit_deleteSwapchain(VkSwapchainKHR *swapchain, struct vInit_imageDetails *imageArray, VkDevice device);
//void vInit_deleteDevice(VkDevice *device);
//void vInit_deleteSurface(VkSurfaceKHR *surface, VkInstance instance);
//void vInit_deleteDebugMessenger(VkDebugUtilsMessengerEXT *debugMessenger, VkInstance instance);
//void vInit_deleteInstance(VkInstance *instance);
//void vInit_destroyVulkan(struct vInit_graphicsStruct *graphicsPacket);

//statics

static VkResult s_vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);
static VkResult s_vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);
static VKAPI_ATTR VkBool32 VKAPI_CALL s_debugLogCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

static int32_t s_getExtensions(VkExtensionProperties **extensions, uint32_t *extensionsCount);
static int32_t s_getEnabledExtensionNames(char ***extensionNameArray, int32_t *extensionNamesCount, int32_t debugMode);
static void s_freeEnabledExtensionNames(char ***extensionNameArray);
static void s_freeExtensions(VkExtensionProperties **extensions);

static int32_t s_getLayers(VkLayerProperties **layers, uint32_t *layersCount);
static int32_t s_evaluateEnabledLayers(const char *const *enabledLayerNames, uint32_t enabledLayerNamesCount, const VkLayerProperties *availableLayers, uint32_t availableLayersCount);
static void s_freeLayers(VkLayerProperties **layers);


static int32_t s_getPhysicalDevices(VkPhysicalDevice **PDs, uint32_t *PDsCount, VkInstance instance);
static int32_t s_evaluatePhysicalDevices(VkPhysicalDevice *physicalDevice, VkPhysicalDevice *PDs, uint32_t PDsCount, VkSurfaceKHR surface);
static int32_t s_ratePhysicalDevice(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
static void s_freePhysicalDevices(VkPhysicalDevice **PDs);


static int32_t s_getQueueFamiliesIndices(struct vInit_queueIndices *queueIndices, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
static int32_t s_getQueueFamiliesProperties(VkQueueFamilyProperties **queueProperties, uint32_t *queuePropertiesCount, VkPhysicalDevice physicalDevice);
static int32_t s_evaluateQueueFamiliesProperties(struct vInit_queueIndices *queueIndicesArg, const VkQueueFamilyProperties *queueProperties, uint32_t queuePropertiesCount, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
static void s_freeQueueFamiliesProperties(VkQueueFamilyProperties **queueProperties);

static int32_t s_getDeviceExtentionProperties(VkExtensionProperties **extensionProperties, uint32_t *extensionPropertiesCount, VkPhysicalDevice physicalDevice);
static int32_t s_evaluateEnabledDeviceExtentionProperties(const char *const *enabledExtensionNames, uint32_t enabledExtensionNamesCount, VkExtensionProperties *extensionProperties, uint32_t extensionPropertiesCount);
static void s_freeDeviceExtentionProperties(VkExtensionProperties **extensionProperties);

static int32_t s_evaluateSwapchainSupport(struct vInit_swapchainSupportDetails *swapchainSupport, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
static int32_t s_getSwapchainSupport(struct vInit_swapchainSupportDetails *swapchainSupport, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
static void s_freeSwapchainSupport(struct vInit_swapchainSupportDetails *swapchainSupport);

static VkSurfaceFormatKHR s_evaluateSurfaceFormats(const VkSurfaceFormatKHR *surfaceFormats, int32_t surfaceFormatsCount);
static VkPresentModeKHR s_evaluatePresentModes(const VkPresentModeKHR *presentModes, int32_t presentModesCount);
static VkExtent2D s_evaluateSwapExtent(const VkSurfaceCapabilitiesKHR *surfaceCapabilities, struct window_window *window);

static int32_t s_getSwapchainImages(struct vInit_imageDetails *imageArray, VkDevice device, VkSwapchainKHR swapchain);
static void s_deleteSwapchainImages(struct vInit_imageDetails *restrict imageDetails);


/*------------------    globals    ------------------*/


const char *const enabledLayerNames[]={"VK_LAYER_KHRONOS_validation"};
const uint32_t enabledLayerNamesCount=sizeof(enabledLayerNames)/sizeof(int8_t*);

const char *const enabledDeviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
const uint32_t enabledDeviceExtensionsCount=sizeof(enabledDeviceExtensions)/sizeof(int8_t*);
    


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
vInit_initVulkan(struct vInit_graphicsStruct *restrict const graphicsPacket, struct window_window *restrict const window){
    VkPhysicalDevice physicalDevice;
    
    if(vInit_createInstance(&(graphicsPacket->instance), graphicsPacket->debugMode)){
        fprintf(stderr, "Error: Creating instance\n");
        return 1;
    }
    if(vInit_createDebugMessenger(&(graphicsPacket->debugMessenger), graphicsPacket->debugMode, graphicsPacket->instance)){
        fprintf(stderr, "Error: Creating debug messenger\n");
        return 1;
    }
    if(vInit_createSurface(&(graphicsPacket->surface), graphicsPacket->instance, window)){
        fprintf(stderr, "Error: Creating the surface\n");
        return 1;
    }
    if(vInit_selectPhysicalDevice(&(graphicsPacket->physicalDevice), graphicsPacket->instance, graphicsPacket->surface)){
        fprintf(stderr, "Error: Chossing physical device\n");
        return 1;
    }
    if(vInit_createDevice(&(graphicsPacket->device), &(graphicsPacket->queueIndices), &(graphicsPacket->queueHandles), graphicsPacket->instance, graphicsPacket->surface, graphicsPacket->physicalDevice)){
        fprintf(stderr, "Error: Creating logical device\n");
        return 1;
    }
    if(vInit_createSwapchain(&(graphicsPacket->swapchain), &(graphicsPacket->swapchainDetails), &(graphicsPacket->imageArray), window, graphicsPacket->surface, graphicsPacket->physicalDevice, &(graphicsPacket->queueIndices), graphicsPacket->device)){
        fprintf(stderr, "Error: Creating swap chain\n");
        return 1;
    }
    if(vInit_createImageViews(&(graphicsPacket->imageViewArray), &(graphicsPacket->imageArray), graphicsPacket->device, &(graphicsPacket->swapchainDetails.imageFormat))){
        fprintf(stderr, "Error: Creating image views\n");
        return 1;
    }
    
    //printf("instance   :%p\n", graphicsPacket->instance);
    //printf("surface    :%p\n", graphicsPacket->surface);
    //printf("device     :%p\n", graphicsPacket->device);
    //printf("physDevice :%p\n", graphicsPacket->physicalDevice);
    return 0;
}

void
vInit_destroyVulkan(struct vInit_graphicsStruct *graphicsPacket){
    vInit_deleteImageViews(&(graphicsPacket->imageViewArray), graphicsPacket->device);
    vInit_deleteSwapchain(&(graphicsPacket->swapchain), &(graphicsPacket->imageArray), graphicsPacket->device);
    vInit_deleteDevice(&(graphicsPacket->device));
    vInit_deleteSurface(&(graphicsPacket->surface), graphicsPacket->instance);
    vInit_deleteDebugMessenger(&(graphicsPacket->debugMessenger), graphicsPacket->instance);
    vInit_deleteInstance(&(graphicsPacket->instance));
}

int32_t 
vInit_createInstance(VkInstance *instance, int32_t debugMode){
    VkApplicationInfo appInfo={};
    VkInstanceCreateInfo imageViewCreateInfo={};
    VkResult vulkanResult;
    int32_t result;
    
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "testEngine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 1, 0);
    appInfo.pEngineName = "testosteroneEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;
    
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    imageViewCreateInfo.pApplicationInfo = &appInfo;
    
    uint32_t enabledExtensionNamesCount = 0;
    char **enabledExtensionNames;
    
    s_getEnabledExtensionNames(&enabledExtensionNames, &enabledExtensionNamesCount, debugMode);
    
    
    //List window required extentions
    printf("Extentions required by Window Manager and debug if apply\n");
    for(int32_t iter=0; iter<enabledExtensionNamesCount; iter++){
        printf("\t%s\n", *(enabledExtensionNames+iter));
    }
    
    imageViewCreateInfo.enabledExtensionCount = enabledExtensionNamesCount;
    imageViewCreateInfo.ppEnabledExtensionNames = (const char* const*)enabledExtensionNames;
    
    VkExtensionProperties *extensions;
    uint32_t extensionsCount;
    if(s_getExtensions(&extensions, &extensionsCount)){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    
    printf("All extentions\n");
    for(int32_t iter=0; iter<extensionsCount; iter++){
        printf("\t%s\n", (extensions+iter)->extensionName);
    }
    
    s_freeExtensions(&extensions);
    
    VkLayerProperties *layers;
    uint32_t layersCount;
    if(s_getLayers(&layers, &layersCount)){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    
    printf("All layers\n");
    for(int32_t iter=0; iter<layersCount; iter++){
        printf("\t%s\n", (layers+iter)->layerName);
    }
    
    result = s_evaluateEnabledLayers(enabledLayerNames, enabledLayerNamesCount, layers, layersCount);
    s_freeLayers(&layers);
    if(result){
        fprintf(stderr, "Error: Not all required layers were available\n");
        return 1;
    }
    
    imageViewCreateInfo.enabledLayerCount = enabledLayerNamesCount;
    imageViewCreateInfo.ppEnabledLayerNames = enabledLayerNames;
    
    if((vulkanResult = vkCreateInstance(&imageViewCreateInfo, NULL, instance)) != VK_SUCCESS){
        fprintf(stderr,"Error: Instance creation error %d\n", vulkanResult);
        return 1;
    }
    
    s_freeEnabledExtensionNames(&enabledExtensionNames);
    
    return 0;
}

void 
vInit_deleteInstance(VkInstance *instance){
    printf("deleting the instance\n");
    vkDestroyInstance(*instance, NULL);
    *instance = (VkInstance){0};
}

int32_t
vInit_createDebugMessenger(VkDebugUtilsMessengerEXT *debugMessenger, int32_t debugMode, VkInstance instance){
    if(!debugMode){
        return 0;
    }
    printf("debug messenger enabled\n");
    
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = s_debugLogCallback;
    createInfo.pUserData = NULL; 
    
    if((s_vkCreateDebugUtilsMessengerEXT(instance, &createInfo, NULL, debugMessenger)) != VK_SUCCESS){
        fprintf(stderr, "Error: Creating debug messenger\n");
        return 1;
    }
    return 0;
}

void
vInit_deleteDebugMessenger(VkDebugUtilsMessengerEXT *debugMessenger, VkInstance instance){
    s_vkDestroyDebugUtilsMessengerEXT(instance, *debugMessenger, NULL);
    *debugMessenger = (VkDebugUtilsMessengerEXT){0};
}

int32_t
vInit_createSurface(VkSurfaceKHR *surface, VkInstance instance, struct window_window *window){
    VkResult result;
    if((result = window_createSurface(surface, window, instance)) != VK_SUCCESS){
        fprintf(stderr, "Error: Creating the surface Errco %d\n", result);
        return 1;
    }
    return 0;
}

void
vInit_deleteSurface(VkSurfaceKHR *surface, VkInstance instance){
    printf("deleting the surface\n");
    vkDestroySurfaceKHR(instance, *surface, NULL);
    *surface = (VkSurfaceKHR){0};
}

int32_t 
vInit_selectPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, VkSurfaceKHR surface){
    int32_t result;
    VkPhysicalDevice *physicalDevices;
    uint32_t physicalDevicesCount=0;
    if(s_getPhysicalDevices(&physicalDevices, &physicalDevicesCount, instance)){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    
    if(physicalDevicesCount==0){
        fprintf(stderr, "Error: No GPU with vulkan suport was found\n");
        return 1;
    }
    
    printf("deviceCount : %d\n", physicalDevicesCount);
    
    result = s_evaluatePhysicalDevices(physicalDevice, physicalDevices, physicalDevicesCount, surface);
    s_freePhysicalDevices(&physicalDevices);
    
    if(result){
        fprintf(stderr, "Error: No suitable GPU available\n");
        return 1;
    }
    printf("suitable: Yes\n");
    return 0;
}

int32_t
vInit_createDevice(VkDevice *device, struct vInit_queueIndices *queueIndices, struct vInit_queueHandles *queueHandles, VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice){
    VkResult result;
    
    if(s_getQueueFamiliesIndices(queueIndices, surface, physicalDevice)){
        fprintf(stderr, "Error: The requiered queues are not supported\n");
        return 1;
    }
    
    if(queueIndices->presentQueue == queueIndices->graphicsQueue){
        printf("same queue\n");
    }
    
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueIndices->presentQueue;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    
    VkPhysicalDeviceFeatures deviceFeatures = {};
    
    VkDeviceCreateInfo deviceCreateInfo = {};
    
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    
    deviceCreateInfo.enabledExtensionCount = enabledDeviceExtensionsCount;
    deviceCreateInfo.ppEnabledExtensionNames = enabledDeviceExtensions;
    
    if(0){//validation layers disabled
        deviceCreateInfo.enabledLayerCount = 0;
    }else{
        deviceCreateInfo.enabledLayerCount = 0;
    }
    
    if((result = vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, device)) != VK_SUCCESS){
        printf("Error: Creating device %d\n", result);
        return 1;
    }else{
        printf("device created succesfully\n");
    }
    vkGetDeviceQueue(*device, queueIndices->presentQueue, 0, &(queueHandles->presentQueue));
    vkGetDeviceQueue(*device, queueIndices->graphicsQueue, 0, &(queueHandles->graphicsQueue));
    return 0;
}

void
vInit_deleteDevice(VkDevice *device){
    vkDestroyDevice(*device, NULL);
    *device = (VkDevice){0};
}

int32_t
vInit_createSwapchain(VkSwapchainKHR *swapchain, struct vInit_swapchainDetails *swapchainDetails, struct vInit_imageDetails *imageDetails, struct window_window *window, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, struct vInit_queueIndices *queueIndices, VkDevice device){
    struct vInit_swapchainSupportDetails swapchainSupportDetails = {0};
    if(s_getSwapchainSupport(&swapchainSupportDetails, surface, physicalDevice)){
        fprintf(stderr, "Error: Not the right support for swapchain\n");
        return 1;
    }
    
    
    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
    VkExtent2D swapExtent;
    
    surfaceFormat = s_evaluateSurfaceFormats(swapchainSupportDetails.surfaceFormats, swapchainSupportDetails.surfaceFormatsCount);
    presentMode = s_evaluatePresentModes(swapchainSupportDetails.presentModes, swapchainSupportDetails.presentModesCount);
    swapExtent = s_evaluateSwapExtent(&(swapchainSupportDetails.surfaceCapabilities), window);
    
    uint32_t imageCount = swapchainSupportDetails.surfaceCapabilities.minImageCount+1;
    
    if(swapchainSupportDetails.surfaceCapabilities.maxImageCount > 0 && imageCount > swapchainSupportDetails.surfaceCapabilities.maxImageCount){
        imageCount = swapchainSupportDetails.surfaceCapabilities.maxImageCount;
    }
    
    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.surface = surface;
    
    swapchainCreateInfo.minImageCount = imageCount;
    swapchainCreateInfo.imageFormat = surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent = swapExtent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    
    int32_t queueFamilyIndices[] = {queueIndices->presentQueue, queueIndices->graphicsQueue};
    
    if(queueIndices->graphicsQueue == queueIndices->presentQueue){
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = NULL;
    }else{
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    
    swapchainCreateInfo.preTransform = swapchainSupportDetails.surfaceCapabilities.currentTransform; 
    
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.clipped = VK_TRUE;
    
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
    
    if((vkCreateSwapchainKHR(device, &swapchainCreateInfo, NULL, swapchain)) != VK_SUCCESS){
        fprintf(stderr, "Error: Creating swap chain\n");
        return 1;
    }
    
    if(s_getSwapchainImages(imageDetails, device, *swapchain)){
        fprintf(stderr, "Error: Getting swapchain images\n");
        return 1;
    }
    swapchainDetails->imageFormat = surfaceFormat.format;
    swapchainDetails->extent = swapExtent;
    
    s_freeSwapchainSupport(&swapchainSupportDetails);
    return 0;
    
}

void 
vInit_deleteSwapchain(VkSwapchainKHR *swapchain, struct vInit_imageDetails *imageArray, VkDevice device){
    s_deleteSwapchainImages(imageArray);
    vkDestroySwapchainKHR(device, *swapchain, NULL);
}


int32_t
vInit_createImageViews(struct vInit_imageViewDetails *restrict imageViewArray, const struct vInit_imageDetails *const restrict imageArray, restrict VkDevice device, const VkFormat *restrict const swapchainImageFormat){
    imageViewArray->imageViews = malloc(imageArray->count*sizeof(VkImageView));
    if(!imageViewArray->imageViews){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    
    imageViewArray->count = imageArray->count;
    
    VkImageViewCreateInfo imageViewCreateInfo = {};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = *swapchainImageFormat;
    
    
    imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    
    imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;
    
    for(int32_t iter=0; iter<imageArray->count; iter++){
        imageViewCreateInfo.image = *((imageArray->images)+iter);
        if(vkCreateImageView(device, &imageViewCreateInfo, NULL, ((imageViewArray->imageViews)+iter))){
            free(imageViewArray->imageViews);
            imageViewArray->imageViews = NULL;
            fprintf(stderr, "Error: Error creating %d view\n", iter);
            return 1;
        }
    }
    
    return 0;
}

/*
void vInit_deleteImageViews(struct vInit_imageViewDetails *imageViewArray, VkDevice device);
*/
void
vInit_deleteImageViews(struct vInit_imageViewDetails *restrict const imageViewArray, restrict VkDevice device){
    for(int32_t iter=0; iter<imageViewArray->count; iter++){
        vkDestroyImageView(device, *((imageViewArray->imageViews)+iter), NULL);
    }
    free(imageViewArray->imageViews);
    imageViewArray->imageViews = NULL;
    imageViewArray->count = 0;
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

//official names of parameters only added the s_ too keep coherence
static
VkResult
s_vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger){
    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXTFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"); 
    //printf("%p\n", vkCreateDebugUtilsMessengerEXTFunc);
    if(vkCreateDebugUtilsMessengerEXTFunc != NULL) {
        return vkCreateDebugUtilsMessengerEXTFunc(instance, pCreateInfo, pAllocator, pMessenger);
    }else{
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static
VkResult
s_vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator){
    PFN_vkDestroyDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXTFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"); 
    //printf("%p\n", vkCreateDebugUtilsMessengerEXTFunc);
    if(vkCreateDebugUtilsMessengerEXTFunc != NULL) {
        vkCreateDebugUtilsMessengerEXTFunc(instance, messenger, pAllocator);
        return 0;
    }else{
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static 
VKAPI_ATTR VkBool32 VKAPI_CALL
s_debugLogCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData){
    
    if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        fprintf(stderr, "\n\n\t\t\t-------%s\n", pCallbackData->pMessage);
    }    
    
    return VK_FALSE;
}

/*
static 
VKAPI_ATTR VkBool32 VKAPI_CALL
s_debugLogCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData){
    switch(messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:{
            fprintf(stderr, "\\\\\\\\-------%s\n", pCallbackData->pMessage);
            break;
        }
        default:{
            //fprintf(stderr, "test message\n");
        }
    }    
    
    return VK_FALSE;
}
*/

static
int32_t
s_evaluateEnabledLayers(const char *const *enabledLayerNames, uint32_t enabledLayerNamesCount, const VkLayerProperties *availableLayers, uint32_t availableLayersCount){
    int32_t found;
    for(int32_t iter=0; iter<enabledLayerNamesCount; iter++){
        found=0;
        for(int32_t iterAvLay=0; iterAvLay<availableLayersCount; iterAvLay++){
            if(!strcmp(*(enabledLayerNames+(iter)), (availableLayers+iterAvLay)->layerName)){
                found=1;
            }
        }
        if(!found){
            return 1;
        }
        
    }
    return 0;
}

static 
int32_t 
s_getExtensions(VkExtensionProperties **extensions, uint32_t *extensionsCount){
    vkEnumerateInstanceExtensionProperties(NULL, extensionsCount, NULL);
    *extensions = malloc((*extensionsCount)*sizeof(VkExtensionProperties));
    if(!(*extensions)){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    vkEnumerateInstanceExtensionProperties(NULL, extensionsCount, *extensions);
    return 0;
}

static
int32_t
s_getEnabledExtensionNames(char ***extensionNamesArray, int32_t *extensionNamesCount, int32_t debugMode){
    uint32_t realExtensionCount;
    uint32_t windowExtensionsCount = 0;
    const char **windowExtensions;
    if(window_getRequiredInstanceExtentions(&windowExtensions, &windowExtensionsCount)){
        fprintf(stderr, "Error: Counldn't retrive extentions\n");
        return 1;
    }
    
    realExtensionCount = windowExtensionsCount;
    if(debugMode){
        realExtensionCount++;
    }
    
    *extensionNamesArray = malloc((realExtensionCount+1)*sizeof(const char*));
    if(!*extensionNamesArray){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    
    for(int32_t iter=0; iter<windowExtensionsCount; iter++){
        printf("hola\n");
        *((*extensionNamesArray)+iter) = calloc(1,VK_MAX_EXTENSION_NAME_SIZE*sizeof(char));
        strcpy(*((*extensionNamesArray)+iter), *(windowExtensions+iter));
    }
    
    if(debugMode){
        *((*extensionNamesArray)+realExtensionCount-1) = calloc(1,VK_MAX_EXTENSION_NAME_SIZE*sizeof(char));
        strcpy(*((*extensionNamesArray)+realExtensionCount-1), VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        *((*extensionNamesArray)+realExtensionCount) = NULL;
    }else{
        *((*extensionNamesArray)+realExtensionCount) = NULL;
    }
    *extensionNamesCount = realExtensionCount;
    
    return 0;
}

static
void
s_freeEnabledExtensionNames(char ***extensionNameArray){
    int32_t iter = 0;
    while(*((*extensionNameArray)+iter)){
        free(*((*extensionNameArray)+iter));
        iter++;
    }
    free(*extensionNameArray);
    *extensionNameArray = NULL;
}

static 
void 
s_freeExtensions(VkExtensionProperties **extensions){
    free(*extensions);
    *extensions=NULL;
}

static 
int32_t 
s_getLayers(VkLayerProperties **layers, uint32_t *layersCount){
    vkEnumerateInstanceLayerProperties(layersCount, NULL);
    *layers = malloc((*layersCount)*sizeof(VkLayerProperties));
    if(!*layers){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    vkEnumerateInstanceLayerProperties(layersCount, *layers);
    return 0;
}

static 
void 
s_freeLayers(VkLayerProperties **layers){
    free(*layers);
    *layers=NULL;
}

//PD stands for Physical Devices
static 
int32_t 
s_getPhysicalDevices(VkPhysicalDevice **PDs, uint32_t *PDsCount, VkInstance instance){
    vkEnumeratePhysicalDevices(instance, PDsCount, NULL);
    *PDs = malloc((*PDsCount)*sizeof(VkPhysicalDevice));
    if(!*PDs){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    vkEnumeratePhysicalDevices(instance, PDsCount, *PDs);
    return 0;
}

static
int32_t
s_evaluatePhysicalDevices(VkPhysicalDevice *physicalDevice, VkPhysicalDevice *PDs, uint32_t PDsCount, VkSurfaceKHR surface){
    int32_t suitableGPU=1;
    for(int iter=0; iter<PDsCount; iter++){
        if(!s_ratePhysicalDevice(surface, *(PDs+iter))){
            *physicalDevice=*(PDs+iter);
            suitableGPU=0;
        }
    }
    return suitableGPU;
}

static
int32_t
s_ratePhysicalDevice(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice){
    int32_t result;
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
    printf("%s\n", deviceProperties.deviceName);
    
    struct vInit_queueIndices queueIndices;
    if(s_getQueueFamiliesIndices(&queueIndices, surface, physicalDevice)){
        fprintf(stderr, "Error: Validating queues\n");
        return 1;
    }
    
    VkExtensionProperties *extensionProperties;
    uint32_t extensionPropertiesCount;
    
    if(s_getDeviceExtentionProperties(&extensionProperties, &extensionPropertiesCount, physicalDevice)){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    
    
#ifdef DEBUG
    printf("%d\n", extensionPropertiesCount);
    for(int32_t iter=0; iter<extensionPropertiesCount; iter++){
        printf("%s\n", (extensionProperties+iter)->extensionName);
    }
#endif
    
    result = s_evaluateEnabledDeviceExtentionProperties(enabledDeviceExtensions, enabledDeviceExtensionsCount, extensionProperties, extensionPropertiesCount);
    s_freeDeviceExtentionProperties(&extensionProperties);
    if(result){
        fprintf(stderr, "Error: No swap chain support\n");
        return 1;
    }
    
    if(s_evaluateSwapchainSupport(NULL, surface, physicalDevice)){
        fprintf(stderr, "Error: Not the right swapchain suport");
        return 1;
    }
    
    printf("swap chain supported\n");
    return 0;
}

static 
void 
s_freePhysicalDevices(VkPhysicalDevice **PDs){
    free(*PDs);
    *PDs=NULL;
}

static 
int32_t 
s_getQueueFamiliesIndices(struct vInit_queueIndices *queueIndices, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice){
    int32_t result;
    VkQueueFamilyProperties *queueProperties;
    uint32_t queuePropertiesCount;
    if(s_getQueueFamiliesProperties(&queueProperties, &queuePropertiesCount, physicalDevice)){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    
    result = s_evaluateQueueFamiliesProperties(queueIndices, queueProperties, queuePropertiesCount, surface, physicalDevice);
    s_freeQueueFamiliesProperties(&queueProperties);
    if(result){
        fprintf(stderr, "Error: Not all required queues were found\n");
        return 1;
    }
    return 0;
}

static
int32_t
s_getQueueFamiliesProperties(VkQueueFamilyProperties **queueProperties, uint32_t *queuePropertiesCount, VkPhysicalDevice physicalDevice){
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, queuePropertiesCount, NULL);
    *queueProperties = malloc(*queuePropertiesCount*sizeof(VkQueueFamilyProperties));
    if(!*queueProperties){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, queuePropertiesCount, *queueProperties);
    return 0;
}

static
int32_t 
s_evaluateQueueFamiliesProperties(struct vInit_queueIndices *queueIndicesArg, const VkQueueFamilyProperties *queueProperties, uint32_t queuePropertiesCount, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice){
    int32_t foundQueueBitmask = 0;
    struct vInit_queueIndices queueIndices = {-1, -1};
    //printf("-------=-=-=-=-=-=-= %d\n", queuePropertiesCount);
    for(int32_t iter=0; iter<queuePropertiesCount; iter++){
        VkBool32 presentSupport = 0;
        if(queueIndices.graphicsQueue == -1){
            if((queueProperties+iter)->queueFlags & VK_QUEUE_GRAPHICS_BIT){
                queueIndices.graphicsQueue = iter;
            }
        }
        if(queueIndices.presentQueue == -1){
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, iter, surface, &presentSupport);
            if(presentSupport){
                queueIndices.presentQueue = iter;
            }
        }
        if(queueIndices.presentQueue != -1 && queueIndices.graphicsQueue != -1){
            *queueIndicesArg = queueIndices;
            //printf("queue index %d %d\n", queueIndices.presentQueue, queueIndices.graphicsQueue);
            return 0;
        }
    }
    return 1;
}

static
void
s_freeQueueFamiliesProperties(VkQueueFamilyProperties **queueProperties){
    free(*queueProperties);
    *queueProperties=NULL;
}

static
int32_t
s_getDeviceExtentionProperties(VkExtensionProperties **extensionProperties, uint32_t *extensionPropertiesCount, VkPhysicalDevice physicalDevice){
    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, extensionPropertiesCount, NULL);
    *extensionProperties = malloc(*extensionPropertiesCount*sizeof(VkExtensionProperties));
    if(!*extensionProperties){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, extensionPropertiesCount, *extensionProperties);
    return 0;
}

static
void
s_freeDeviceExtentionProperties(VkExtensionProperties **extensionProperties){
    free(*extensionProperties);
    *extensionProperties=NULL;
}

static
int32_t
s_evaluateEnabledDeviceExtentionProperties(const char *const *enabledExtensionNames, uint32_t enabledExtensionNamesCount, VkExtensionProperties *extensionProperties, uint32_t extensionPropertiesCount){
    int32_t found;
    for(int32_t iter=0; iter<enabledExtensionNamesCount; iter++){
        //printf("%s ", enabledExtensionNames[iter]);
        for(int32_t iterAvExt=0; iterAvExt<extensionPropertiesCount; iterAvExt++){
            if(!strcmp(*(enabledExtensionNames+iter), (extensionProperties+iterAvExt)->extensionName)){
                //printf("se encontro el swapchain\n");
                found=1;
            }
            
            //printf("%d %d %s\n", iter, iterAvExt, (extensionProperties+iterAvExt)->extensionName);
        }
    }
    return !found;
}

//if first parameter is NULL it will only check for numbers
static
int32_t
s_evaluateSwapchainSupport(struct vInit_swapchainSupportDetails *swapchainSupport, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice){
    VkSurfaceCapabilitiesKHR capabilitiesTmp;
    VkSurfaceCapabilitiesKHR *capabilitiesHolder;
    
    int32_t presentModesCountTmp;
    int32_t *presentModesCountHolder;
    
    int32_t surfaceFormatsCountTmp;
    int32_t *surfaceFormatsCountHolder;
    
    if(swapchainSupport){
        capabilitiesHolder = &(swapchainSupport->surfaceCapabilities);
        presentModesCountHolder = &(swapchainSupport->presentModesCount);
        surfaceFormatsCountHolder = &(swapchainSupport->surfaceFormatsCount);
    }else{
        capabilitiesHolder = &capabilitiesTmp;
        presentModesCountHolder = &presentModesCountTmp;
        surfaceFormatsCountHolder = &surfaceFormatsCountTmp;
    }
    
    if((vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, capabilitiesHolder)) != VK_SUCCESS){
        return 1;
    }
    
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, surfaceFormatsCountHolder, NULL);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, presentModesCountHolder, NULL);
    
    if(!*presentModesCountHolder || !*surfaceFormatsCountHolder){
        
        return 1;
    } 
    
    return 0;
}

static
int32_t
s_getSwapchainSupport(struct vInit_swapchainSupportDetails *swapchainSupport, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice){
    swapchainSupport->surfaceFormats = NULL;
    swapchainSupport->presentModes = NULL;
    
    if(s_evaluateSwapchainSupport(swapchainSupport, surface, physicalDevice)){
        return 1;
    }
    
    if(swapchainSupport->surfaceFormatsCount){
        swapchainSupport->surfaceFormats = malloc(swapchainSupport->surfaceFormatsCount*sizeof(VkSurfaceFormatKHR));
        if(!(swapchainSupport->surfaceFormats)){
            fprintf(stderr, "Error: Malloc failed\n");
            return 1;
        }
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &(swapchainSupport->surfaceFormatsCount), swapchainSupport->surfaceFormats);
    }
    
    if(swapchainSupport->surfaceFormatsCount){
        swapchainSupport->presentModes = malloc(swapchainSupport->presentModesCount*sizeof(VkPresentModeKHR));
        if(!(swapchainSupport->presentModes)){
            fprintf(stderr, "Error: Malloc failed\n");
            return 1;
        }
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &(swapchainSupport->presentModesCount), swapchainSupport->presentModes);
    }
    
    return 0;
}

static
void
s_freeSwapchainSupport(struct vInit_swapchainSupportDetails *swapchainSupport){
    free(swapchainSupport->surfaceFormats);
    free(swapchainSupport->presentModes);
    swapchainSupport->surfaceFormats = NULL;
    swapchainSupport->presentModes = NULL;
}


static
VkSurfaceFormatKHR 
s_evaluateSurfaceFormats(const VkSurfaceFormatKHR *surfaceFormats, int32_t surfaceFormatsCount){
    for(int32_t iter=0; iter<surfaceFormatsCount; iter++){
        if(((surfaceFormats+iter)->format == VK_FORMAT_B8G8R8A8_SRGB) && ((surfaceFormats+iter)->colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)){
            printf(" * : Prefered Surface Formats\n");
            return *(surfaceFormats+iter);
        }
    }
    printf("   : Prefered Surface Formats\n");
    return *surfaceFormats;
}

static
VkPresentModeKHR
s_evaluatePresentModes(const VkPresentModeKHR *presentModes, int32_t presentModesCount){
    VkPresentModeKHR best;
    for(int32_t iter=0; iter<presentModesCount; iter++){
        if(*(presentModes+iter) == VK_PRESENT_MODE_MAILBOX_KHR){
            printf(" * : Prefered present mode\n");
            return *(presentModes+iter);
        }
    }
    printf(" 0 : Prefered present mode\n");
    return VK_PRESENT_MODE_FIFO_KHR;
}

static
VkExtent2D
s_evaluateSwapExtent(const VkSurfaceCapabilitiesKHR *surfaceCapabilities, struct window_window *window){
    if(surfaceCapabilities->currentExtent.width != (int32_t)0xFFFFFFFF){
        printf(" * : Swap Extent of size %d %d\n", surfaceCapabilities->currentExtent.width, surfaceCapabilities->currentExtent.height);
        return surfaceCapabilities->currentExtent;
    }else{
        int32_t width, height;
        window_getFrameBufferSize(window, &width, &height);
        //printf("max\n\t%d\n\t%d\n", surfaceCapabilities->maxImageExtent.height, surfaceCapabilities->maxImageExtent.width);
        //printf("min\n\t%d\n\t%d\n", surfaceCapabilities->minImageExtent.height, surfaceCapabilities->minImageExtent.width);
        //printf("current\n\t%d\n\t%d\n", surfaceCapabilities->currentExtent.height, surfaceCapabilities->currentExtent.width);
        
        if(width < surfaceCapabilities->minImageExtent.width){
            width = surfaceCapabilities->minImageExtent.width;
        }else if(width > surfaceCapabilities->maxImageExtent.width){
            width = surfaceCapabilities->maxImageExtent.width;
        }
        
        if(height < surfaceCapabilities->minImageExtent.height){
            height = surfaceCapabilities->minImageExtent.height;
        }else if(height > surfaceCapabilities->maxImageExtent.height){
            height = surfaceCapabilities->maxImageExtent.height;
        }
        
        printf(" / : Swap Extent of size %d %d\n", width, height);
        VkExtent2D ret = (VkExtent2D){.width=width, .height=height};
        return ret;
    }
}

static
int32_t
s_getSwapchainImages(struct vInit_imageDetails *imageArray, VkDevice device, VkSwapchainKHR swapchain){
    vkGetSwapchainImagesKHR(device, swapchain, &(imageArray->count), NULL);
    imageArray->images = malloc(imageArray->count*sizeof(VkImage));
    if(!imageArray->images){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    vkGetSwapchainImagesKHR(device, swapchain, &(imageArray->count), imageArray->images);
    return 0;
}

static
void
s_deleteSwapchainImages(struct vInit_imageDetails *restrict imageDetails){
    free(imageDetails->images);
    imageDetails->images = NULL;
}

