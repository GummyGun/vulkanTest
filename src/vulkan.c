#include "vulkan.h"
#include "window.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/*------------------defines------------------*/
#define VULKAN_MAX_LAYER_NAME 64

/*------------------  enums  ------------------*/

/*------------------  stucts  ------------------*/

struct vulkan_queueIndices{
    int32_t graphicsQueue;
    int32_t presentQueue;
};

struct vulkan_swapChainSupportDetails{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    uint32_t surfaceFormatsCount;
    VkSurfaceFormatKHR *surfaceFormats;
    uint32_t presentModesCount;
    VkPresentModeKHR *presentModes;
};

/*------------------prototipes------------------*/

/* instance -> surface -> physicalDevice -> device -> Swapchain */

//int32_t vulkan_initVulkan(struct vulkan_graphicsStruct *graphicsPacket, GLFWwindow *window);
//int32_t vulkan_createInstance(VkInstance *instance);
//int32_t vulkan_createSurface(VkSurfaceKHR *surface, VkInstance instance, GLFWwindow *window);
//int32_t vulkan_selectPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, VkSurfaceKHR surface);
//int32_t vulkan_createLogicalDevice(VkDevice *device, struct vulkan_queueHandles *queueHandles, VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
//int32_t vulkan_createSwapchain(VkSwapchainKHR *swapChain, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkDevice device);

//deletion functions

//void vulkan_deleteLogicalDevice(VkDevice *device);
//void vulkan_deleteSurface(VkSurfaceKHR *surface, VkInstance instance);
//void vulkan_deleteInstance(VkInstance *instance);
//void vulkan_deleteVulkan(struct vulkan_graphicsStruct *graphicsPacket);
//void vulkan_deleteSwapchain(VkSwapchainKHR *swapChain);

//statics




static int32_t s_getExtensions(VkExtensionProperties **extensions, uint32_t *extensionsCount);
static void s_freeExtensions(VkExtensionProperties **extensions);

static int32_t s_getLayers(VkLayerProperties **layers, uint32_t *layersCount);
static int32_t s_evaluateEnabledLayers(const char *const *enabledLayerNames, uint32_t enabledLayerNamesCount, const VkLayerProperties *availableLayers, uint32_t availableLayersCount);
static void s_freeLayers(VkLayerProperties **layers);


static int32_t s_getPhysicalDevices(VkPhysicalDevice **PDs, uint32_t *PDsCount, VkInstance instance);
static int32_t s_evaluatePhysicalDevices(VkPhysicalDevice *physicalDevice, VkPhysicalDevice *PDs, uint32_t PDsCount, VkSurfaceKHR surface);
static int32_t s_ratePhysicalDevice(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
static void s_freePhysicalDevices(VkPhysicalDevice **PDs);


static int32_t s_getQueueFamiliesIndices(struct vulkan_queueIndices *queueIndices, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
static int32_t s_getQueueFamiliesProperties(VkQueueFamilyProperties **queueProperties, uint32_t *queuePropertiesCount, VkPhysicalDevice physicalDevice);
static int32_t s_evaluateQueueFamiliesProperties(struct vulkan_queueIndices *queueIndicesArg, const VkQueueFamilyProperties *queueProperties, uint32_t queuePropertiesCount, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
static void s_freeQueueFamiliesProperties(VkQueueFamilyProperties **queueProperties);

static int32_t s_getDeviceExtentionProperties(VkExtensionProperties **extensionProperties, uint32_t *extensionPropertiesCount, VkPhysicalDevice physicalDevice);
static int32_t s_evaluateEnabledDeviceExtentionProperties(const char *const *enabledExtensionNames, uint32_t enabledExtensionNamesCount, VkExtensionProperties *extensionProperties, uint32_t extensionPropertiesCount);
static void s_freeDeviceExtentionProperties(VkExtensionProperties **extensionProperties);

static int32_t s_evaluateSwapchainSupport(struct vulkan_swapChainSupportDetails *swapChainSupport, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
static int32_t s_getSwapchainSupport(struct vulkan_swapChainSupportDetails *swapChainSupport, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
static void s_freeSwapchainSupport(struct vulkan_swapChainSupportDetails *swapChainSupport);

static VkSurfaceFormatKHR s_evaluateSwapSurfaceFormat(const VkSurfaceFormatKHR *surfaceFormats, int32_t surfaceFormatsCount);
static VkPresentModeKHR s_evaluateSwapPresentMode(const VkPresentModeKHR *presentModes, int32_t presentModesCount);
static VkExtent2D s_evaluateSwapExtent(const VkSurfaceCapabilitiesKHR surfaceCapabilities);


/*------------------    globals    ------------------*/


const char *const enabledLayerNames[]={"VK_LAYER_KHRONOS_validation"};
const uint32_t enabledLayerNamesCount=sizeof(enabledLayerNames)/sizeof(int8_t*);

const char *const enabledDeviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
const uint32_t enabledDeviceExtensionsCount=sizeof(enabledDeviceExtensions)/sizeof(int8_t*);
    


/*------------------implementations------------------*/


int32_t
vulkan_initVulkan(struct vulkan_graphicsStruct *graphicsPacket, GLFWwindow *window){
    VkPhysicalDevice physicalDevice;
    
    if(vulkan_createInstance(&(graphicsPacket->instance))){
        fprintf(stderr, "Error: Creating instance\n");
        return 1;
    }
    if(vulkan_createSurface(&(graphicsPacket->surface), (graphicsPacket->instance), window)){
        fprintf(stderr, "Error: Creating the surface\n");
    }
    if(vulkan_selectPhysicalDevice(&(graphicsPacket->physicalDevice), (graphicsPacket->instance), (graphicsPacket)->surface)){
        fprintf(stderr, "Error: Chossing physical device\n");
        return 1;
    }
    if(vulkan_createLogicalDevice(&(graphicsPacket->device), &(graphicsPacket->queuesHandles), (graphicsPacket->instance), (graphicsPacket->surface), (graphicsPacket->physicalDevice))){
        fprintf(stderr, "Error: Creating logical device\n");
        return 1;
    }
    if(vulkan_createSwapchain(&(graphicsPacket->swapchain), (graphicsPacket->surface), (graphicsPacket->physicalDevice), (graphicsPacket->device))){
        fprintf(stderr, "Error: Creating swap chain\n");
        return 1;
    }
    
    return 0;
}

void
vulkan_deleteVulkan(struct vulkan_graphicsStruct *graphicsPacket){
    vulkan_deleteLogicalDevice(&(graphicsPacket->device));
    vulkan_deleteSurface(&(graphicsPacket->surface), graphicsPacket->instance);
    vulkan_deleteInstance(&(graphicsPacket->instance));
}

int32_t 
vulkan_createInstance(VkInstance *instance){
    VkApplicationInfo appInfo={};
    VkInstanceCreateInfo createInfo={};
    VkResult vulkanResult;
    int32_t result;
    
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "testEngine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 1, 0);
    appInfo.pEngineName = "testosteroneEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;
    
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    uint32_t glfwExtensionsCount = 0;
    const char **glfwExtensions;
    
    if(window_getRequiredInsanceExtentions(&glfwExtensions, &glfwExtensionsCount)){
        fprintf(stderr, "Counldn't retrive extentions\n");
        return 1;
    }
    
    //List glfw required extentions
    printf("Extentions required by GLFW\n");
    for(int32_t iter=0; iter<glfwExtensionsCount; iter++){
        printf("\t%s\n", *(glfwExtensions+iter));
        
    }
    
    createInfo.enabledExtensionCount = glfwExtensionsCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    
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
    
    createInfo.enabledLayerCount = enabledLayerNamesCount;
    createInfo.ppEnabledLayerNames = enabledLayerNames;
    
    vulkanResult = vkCreateInstance(&createInfo, NULL, instance);
    if(vulkanResult != VK_SUCCESS){
        fprintf(stderr,"Error: Instance creation error %d\n", vulkanResult);
        return 1;
    }
    return 0;
}


void 
vulkan_deleteInstance(VkInstance *instance){
    printf("deleting the instance\n");
    vkDestroyInstance(*instance, NULL);
    *instance = (VkInstance){0};
}

int32_t
vulkan_createSurface(VkSurfaceKHR *surface, VkInstance instance, GLFWwindow *window){
    VkResult result;
    if((result = glfwCreateWindowSurface(instance, window, NULL, surface)) != VK_SUCCESS){
        fprintf(stderr, "Error: Creating the surface %d\n", result);
        return 1;
    }
    return 0;
}

void
vulkan_deleteSurface(VkSurfaceKHR *surface, VkInstance instance){
    printf("deleting the surface\n");
    vkDestroySurfaceKHR(instance, *surface, NULL);
    *surface = (VkSurfaceKHR){0};
}

int32_t 
vulkan_selectPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, VkSurfaceKHR surface){
    int32_t result;
    VkPhysicalDevice *physicalDevices;
    uint32_t physicalDevicesCount=0;
    if(s_getPhysicalDevices(&physicalDevices, &physicalDevicesCount, instance)){
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    
    if(physicalDevicesCount==0){
        fprintf(stderr, "no GPU with vulkan suport was found\n");
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
vulkan_createLogicalDevice(VkDevice *device, struct vulkan_queueHandles *queueHandles, VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice){
    VkResult result;
    
    struct vulkan_queueIndices queueIndices;
    
    if(s_getQueueFamiliesIndices(&queueIndices, surface, physicalDevice)){
        fprintf(stderr, "Error: The requiered queues are not supported\n");
        return 1;
    }
    
    if(queueIndices.presentQueue == queueIndices.graphicsQueue){
        printf("same queue\n");
    }
    
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueIndices.presentQueue;
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
    vkGetDeviceQueue(*device, queueIndices.presentQueue, 0, &(queueHandles->presentQueue));
    vkGetDeviceQueue(*device, queueIndices.graphicsQueue, 0, &(queueHandles->graphicsQueue));
    return 0;
}

void
vulkan_deleteLogicalDevice(VkDevice *device){
    vkDestroyDevice(*device, NULL);
    *device = (VkDevice){0};
}

int32_t
vulkan_createSwapchain(VkSwapchainKHR *swapChain, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkDevice device){
    struct vulkan_swapChainSupportDetails swapChainDetails = {0};
    s_getSwapchainSupport(&swapChainDetails, surface, physicalDevice);
    //printf("%d %d\n", swapChainDetails.surfaceFormatsCount, swapChainDetails.presentModesCount);
    
    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentationMode;
    VkExtent2D swapExtention;
    
    surfaceFormat = s_evaluateSwapSurfaceFormat(swapChainDetails.surfaceFormats, swapChainDetails.surfaceFormatsCount);
    presentationMode = s_evaluateSwapPresentMode(swapChainDetails.presentModes, swapChainDetails.presentModesCount);
    swapExtention = s_evaluateSwapExtent(swapChainDetails.surfaceCapabilities);
    
    s_freeSwapchainSupport(&swapChainDetails);
    return 1;
    
}

/* ----------------------- static methods ----------------------- */

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
        return 1;
    }
    vkEnumerateInstanceExtensionProperties(NULL, extensionsCount, *extensions);
    return 0;
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
    
    struct vulkan_queueIndices queueIndices;
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
    
    result = s_evaluateEnabledDeviceExtentionProperties(enabledDeviceExtensions, enabledDeviceExtensionsCount, extensionProperties, extensionPropertiesCount);
    s_freeDeviceExtentionProperties(&extensionProperties);
    if(result){
        fprintf(stderr, "Error: No swap chain support\n");
        return 1;
    }else{
        s_evaluateSwapchainSupport(NULL, surface, physicalDevice);
        printf("swap chain supported\n");
    }
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
s_getQueueFamiliesIndices(struct vulkan_queueIndices *queueIndices, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice){
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
        return 1;
    }
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, queuePropertiesCount, *queueProperties);
    return 0;
}

static
int32_t 
s_evaluateQueueFamiliesProperties(struct vulkan_queueIndices *queueIndicesArg, const VkQueueFamilyProperties *queueProperties, uint32_t queuePropertiesCount, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice){
    int32_t foundQueueBitmask = 0;
    struct vulkan_queueIndices queueIndices = {-1, -1};
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
s_evaluateSwapchainSupport(struct vulkan_swapChainSupportDetails *swapChainSupport, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice){
    VkSurfaceCapabilitiesKHR capabilitiesTmp;
    VkSurfaceCapabilitiesKHR *capabilitiesHolder;
    
    int32_t presentModesCountTmp;
    int32_t *presentModesCountHolder;
    
    int32_t surfaceFormatsCountTmp;
    int32_t *surfaceFormatsCountHolder;
    
    if(swapChainSupport){
        capabilitiesHolder = &(swapChainSupport->surfaceCapabilities);
        presentModesCountHolder = &(swapChainSupport->presentModesCount);
        surfaceFormatsCountHolder = &(swapChainSupport->surfaceFormatsCount);
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
s_getSwapchainSupport(struct vulkan_swapChainSupportDetails *swapChainSupport, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice){
    swapChainSupport->surfaceFormats = NULL;
    swapChainSupport->presentModes = NULL;
    
    if(s_evaluateSwapchainSupport(swapChainSupport, surface, physicalDevice)){
        return 1;
    }
    
    if(swapChainSupport->surfaceFormatsCount){
        swapChainSupport->surfaceFormats = malloc(swapChainSupport->surfaceFormatsCount*sizeof(VkSurfaceFormatKHR));
        if(!(swapChainSupport->surfaceFormats)){
            return 1;
        }
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &(swapChainSupport->surfaceFormatsCount), swapChainSupport->surfaceFormats);
    }
    
    if(swapChainSupport->surfaceFormatsCount){
        swapChainSupport->presentModes = malloc(swapChainSupport->presentModesCount*sizeof(VkPresentModeKHR));
        if(!(swapChainSupport->presentModes)){
            return 1;
        }
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &(swapChainSupport->presentModesCount), swapChainSupport->presentModes);
    }
    
    return 0;
}

static
void
s_freeSwapchainSupport(struct vulkan_swapChainSupportDetails *swapChainSupport){
    free(swapChainSupport->surfaceFormats);
    free(swapChainSupport->presentModes);
    swapChainSupport->surfaceFormats = NULL;
    swapChainSupport->presentModes = NULL;
}


static
VkSurfaceFormatKHR 
s_evaluateSwapSurfaceFormat(const VkSurfaceFormatKHR *surfaceFormats, int32_t surfaceFormatsCount){
    printf("they are %d surface Formats\n", surfaceFormatsCount);
    return *surfaceFormats;
}

static
VkPresentModeKHR
s_evaluateSwapPresentMode(const VkPresentModeKHR *presentModes, int32_t presentModesCount){
    printf("they are %d swap Modes\n", presentModesCount);
    return *presentModes;
}

static
VkExtent2D
s_evaluateSwapExtent(const VkSurfaceCapabilitiesKHR surfaceCapabilities){
    printf("evaluationSwapExtent\n");
    VkExtent2D ret = {};
    return ret;
}
