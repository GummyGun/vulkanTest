#include "vulkan.h"
#include "window.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------defines------------------*/
#define VULKAN_MAX_LAYER_NAME 64

/*------------------prototipes------------------*/

//int32_t vulkan_initVulkan(VkInstance *instance, VkDevice *device, VkQueue *graphicsQueue);
//int32_t vulkan_createInstance(VkInstance *instance);
//int32_t vulkan_selectPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance);
//int32_t vulkan_createLogicalDevice(VkDevice *device, VkQueue *graphicsQueue, VkInstance instance, VkPhysicalDevice physicalDevice);

//deletion functions

//int32_t vulkan_deleteVulkan(VkInstance *instance, VkDevice *device);
//int32_t vulkan_deleteInstance(VkInstance *instance);
//void vulkan_deleteLogicalDevice(VkDevice *device);

//statics

static int32_t s_enableLayers(const char **layerNames, uint32_t layerNamesCount, const VkLayerProperties *availableLayers, uint32_t availableLayersCount);
static void s_getExtensions(VkExtensionProperties **extensions, uint32_t *extensionsCount);
static void s_freeExtensions(VkExtensionProperties **extensions);

static void s_getLayers(VkLayerProperties **layers, uint32_t *layersCount);
static void s_freeLayers(VkLayerProperties **layers);


static int32_t s_getPhysicalDevices(VkPhysicalDevice **PDs, uint32_t *PDsCount, VkInstance instance);
static int32_t s_evaluatePhysicalDevices(VkPhysicalDevice *physicalDevice, const VkPhysicalDevice *PDs, uint32_t PDsCount);
static int32_t s_ratePhysicalDevice(VkPhysicalDevice physicalDevice);
static void s_freePhysicalDevices(VkPhysicalDevice **PDs);


static void s_getPhysicalDeviceQueueFamilies(VkQueueFamilyProperties **queueProperties, uint32_t *queuePropertiesCount, VkPhysicalDevice physicalDevice);
static int32_t s_getQueueFamiliesIndex(int32_t *index, const VkQueueFamilyProperties *queueProperties, uint32_t queuePropertiesCount);

static void s_freePhysicalDeviceQueueFamilies(VkQueueFamilyProperties **queueProperties);

/*------------------implementations------------------*/

int32_t 
vulkan_initVulkan(VkInstance *instance, VkDevice *device, VkQueue *graphicsQueue){
    VkPhysicalDevice physicalDevice;
    
    if(vulkan_createInstance(instance)){
        fprintf(stderr, "Error creating instance\n");
        return 1;
    }
    if(vulkan_selectPhysicalDevice(&physicalDevice, *instance)){
        fprintf(stderr, "Error chossing physical device\n");
        return 1;
    }
    if(vulkan_createLogicalDevice(device, graphicsQueue, *instance, physicalDevice)){
        fprintf(stderr, "Error creating logical device\n");
        return 1;
    }
    
    return 0;
}

int32_t 
vulkan_deleteVulkan(VkInstance *instance, VkDevice *device){
    vulkan_deleteLogicalDevice(device);
    
    if(vulkan_deleteInstance(instance)){
        fprintf(stderr, "Error deleting instance\n");
        return 1;
    }
    return 0;
}

int32_t 
vulkan_createInstance(VkInstance *instance){
    VkApplicationInfo appInfo={};
    VkInstanceCreateInfo createInfo={};
    VkResult result;
    
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
    s_getExtensions(&extensions, &extensionsCount);
    printf("All extentions\n");
    for(int32_t iter=0; iter<extensionsCount; iter++){
        printf("\t%s\n", (extensions+iter)->extensionName);
        
    }
    
    VkLayerProperties *layers;
    uint32_t layersCount;
    s_getLayers(&layers, &layersCount);
    printf("All layers\n");
    for(int32_t iter=0; iter<layersCount; iter++){
        printf("\t%s\n", (layers+iter)->layerName);
    }
    
    const char *enabledLayerNames[]={"VK_LAYER_KHRONOS_validation"};
    uint32_t enabledLayerNamesCount=sizeof(enabledLayerNames)/sizeof(int8_t*);
    
    if(s_enableLayers((const char**)enabledLayerNames, enabledLayerNamesCount, layers, layersCount)){
        printf("Not all required layers were available\n");
        return 1;
    }
    
    createInfo.enabledLayerCount = enabledLayerNamesCount;
    createInfo.ppEnabledLayerNames = enabledLayerNames;
    
    if((result = vkCreateInstance(&createInfo, NULL, instance)) != VK_SUCCESS){
        fprintf(stderr,"%d\n", result);
        return 1;
    }
    
    s_freeLayers(&layers);
    s_freeExtensions(&extensions);
    
    return 0;
}


int32_t 
vulkan_deleteInstance(VkInstance *instance){
    printf("deleting the instance\n");
    vkDestroyInstance(*instance, NULL);
    *instance = (VkInstance){0};
    return 0;
}

int32_t 
vulkan_selectPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance){
    printf("creating a physical device\n");
    VkPhysicalDevice *physicalDevices;
    uint32_t physicalDevicesCount=0;
    if(s_getPhysicalDevices(&physicalDevices, &physicalDevicesCount, instance)){
        fprintf(stderr, "no GPU with vulkan suport was found\n");
        return 1;
    }
    printf("deviceCount : %d\n", physicalDevicesCount);
    
    if(s_evaluatePhysicalDevices(physicalDevice, physicalDevices, physicalDevicesCount)){
        fprintf(stderr, "no suitable GPU available\n");
        return 1;
    }else{
        printf("suitable: Yes\n");
    }

    s_freePhysicalDevices(&physicalDevices);
    return 0;
}

int32_t
vulkan_createLogicalDevice(VkDevice *device, VkQueue *graphicsQueue, VkInstance instance, VkPhysicalDevice physicalDevice){
    VkResult result;
    
    VkQueueFamilyProperties *queueProperties;
    uint32_t queuePropertiesCount;
    s_getPhysicalDeviceQueueFamilies(&queueProperties, &queuePropertiesCount, physicalDevice);
    
    int32_t queueFamilyIndex;
    
    if(s_getQueueFamiliesIndex(&queueFamilyIndex ,queueProperties, queuePropertiesCount)){
        fprintf(stderr, "no suitable Queue\n");
        return 1;
    }
    
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    
    VkPhysicalDeviceFeatures deviceFeatures = {};
    
    VkDeviceCreateInfo deviceCreateInfo = {};
    
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    
    deviceCreateInfo.enabledExtensionCount = 0;
    
    if(0){//validation layers disabled
        deviceCreateInfo.enabledLayerCount = 0;
    }else{
        deviceCreateInfo.enabledLayerCount = 0;
    }
    
    if((result = vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, device)) != VK_SUCCESS){
        printf("error creating device %d\n", result);
        return 1;
    }else{
        printf("device created succesfully\n");
    }
    
    vkGetDeviceQueue(*device, queueFamilyIndex, 0, graphicsQueue);
    
    s_freePhysicalDeviceQueueFamilies(&queueProperties);
    return 0;
}

void
vulkan_deleteLogicalDevice(VkDevice *device){
    vkDestroyDevice(*device, NULL);
}

//static methods

static
int32_t
s_enableLayers(const char **layerNames, uint32_t layerNamesCount, const VkLayerProperties *availableLayers, uint32_t availableLayersCount){
    int32_t found;
    for(int32_t iter=0; iter<layerNamesCount; iter++){
        found=0;
        for(int32_t iterAvLay=0; iterAvLay<availableLayersCount; iterAvLay++){
            if(!strcmp(*(layerNames+(iter)), (availableLayers+iterAvLay)->layerName)){
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
void 
s_getExtensions(VkExtensionProperties **extensions, uint32_t *extensionsCount){
    vkEnumerateInstanceExtensionProperties(NULL, extensionsCount, NULL);
    *extensions = malloc((*extensionsCount)*sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(NULL, extensionsCount, *extensions);
}

static 
void 
s_freeExtensions(VkExtensionProperties **extensions){
    free(*extensions);
    *extensions=NULL;
}

static 
void 
s_getLayers(VkLayerProperties **layers, uint32_t *layersCount){
    vkEnumerateInstanceLayerProperties(layersCount, NULL);
    *layers = malloc((*layersCount)*sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(layersCount, *layers);
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
    if(PDsCount==0){
        return 1;
    }
    *PDs = malloc((*PDsCount)*sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(instance, PDsCount, *PDs);
}

static
int32_t
s_evaluatePhysicalDevices(VkPhysicalDevice *physicalDevice, const VkPhysicalDevice *PDs, uint32_t PDsCount){
    int32_t suitableGPU=1;
    for(int iter=0; iter<PDsCount; iter++){
        if(!s_ratePhysicalDevice(*(PDs+iter))){
            *physicalDevice=*(PDs+iter);
            suitableGPU=0;
        }
    }
    return suitableGPU;
}

static
int32_t
s_ratePhysicalDevice(VkPhysicalDevice physicalDevice){
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
    
    printf("%s\n", deviceProperties.deviceName);
    
    return 0;
}

static 
void 
s_freePhysicalDevices(VkPhysicalDevice **PDs){
    free(*PDs);
    *PDs=NULL;
}

static
void
s_getPhysicalDeviceQueueFamilies(VkQueueFamilyProperties **queueProperties, uint32_t *queuePropertiesCount, VkPhysicalDevice physicalDevice){
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, queuePropertiesCount, NULL);
    *queueProperties = malloc(*queuePropertiesCount*sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, queuePropertiesCount, *queueProperties);
}

static
int32_t
s_getQueueFamiliesIndex(int32_t *index, const VkQueueFamilyProperties *queueProperties, uint32_t queuePropertiesCount){
    for(int32_t iter=0; iter<queuePropertiesCount; iter++){
        if((queueProperties+iter)->queueFlags & VK_QUEUE_GRAPHICS_BIT){
            *index = iter;
            return 0;
        }
    }
    return 1;
}

static
void
s_freePhysicalDeviceQueueFamilies(VkQueueFamilyProperties **queueProperties){
    free(*queueProperties);
    *queueProperties=NULL;
}
