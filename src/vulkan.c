#include "vulkan.h"
#include "window.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------defines------------------*/
#define VULKAN_MAX_LAYER_NAME 64

/*------------------prototipes------------------*/

//int32_t vulkan_initVulkan(VkInstance *instance, VkPhysicalDevice *physicalDevice);
//int32_t vulkan_createInstance(VkInstance *instance);
//int32_t vulkan_createPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance);

//deletion functions

//int32_t vulkan_deleteVulkan(VkInstance *instance, VkPhysicalDevice *physicalDevice);
//int32_t vulkan_deleteInstance(VkInstance *instance);
//int32_t vulkan_deletePhysicalDevice(VkPhysicalDevice *physicalDevice);

//statics

static int32_t s_enableLayers(const char **layerNames, int32_t layerNamesCount, VkLayerProperties *availableLayers, uint32_t availableLayersCount);
static void s_getExtensions(VkExtensionProperties **extensions, uint32_t *extensionsCount);
static void s_freeExtensions(VkExtensionProperties **extensions);

static void s_getLayers(VkLayerProperties **layers, uint32_t *layersCount);
static void s_freeLayers(VkLayerProperties **layers);

static int32_t s_getPhysicalDevices(VkPhysicalDevice **PDs, uint32_t *PDsCount, VkInstance instance);
static int8_t s_validatePhysicalDevice(VkPhysicalDevice PD);
static void s_freePhysicalDevices(VkPhysicalDevice **PDs);

/*------------------implementations------------------*/

int32_t 
vulkan_initVulkan(VkInstance *instance, VkPhysicalDevice *physicalDevice){
    if(vulkan_createInstance(instance)){
        fprintf(stderr, "Error creating instance\n");
        return 1;
    }
    if(vulkan_createPhysicalDevice(physicalDevice, *instance)){
        fprintf(stderr, "Error chossing physical device\n");
        return 1;
    };
    
    return 0;
}

int32_t 
vulkan_deleteVulkan(VkInstance *instance, VkPhysicalDevice *physicalDevice){
    if(vulkan_deletePhysicalDevice(physicalDevice)){
        fprintf(stderr, "Error deleting physical device\n");
        return 1;
    }
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
    int32_t enabledLayerNamesCount=sizeof(enabledLayerNames)/sizeof(int8_t*);
    
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
vulkan_createPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance){
    printf("creating a physical device\n");
    VkPhysicalDevice *physicalDevices;
    uint32_t physicalDevicesCount=0;
    if(s_getPhysicalDevices(&physicalDevices, &physicalDevicesCount, instance)){
        fprintf(stderr, "no GPU with vulkan suport was found\n");
        return 1;
    }
    int32_t suitableGPU=0;
    for(int iter=0; iter<physicalDevicesCount; iter++){
        if(!s_validatePhysicalDevice(*(physicalDevices+iter))){
            *physicalDevice=*(physicalDevices+iter);
            suitableGPU=1;
        }
    }
    if(!suitableGPU){
        fprintf(stderr, "no suitable GPU available\n");
        return 1;
    }
    printf("deviceCount : %d\n", physicalDevicesCount);
    s_freePhysicalDevices(&physicalDevices);
    return 0;
}

int32_t 
vulkan_deletePhysicalDevice(VkPhysicalDevice *physicalDevice){
    printf("deleting a physical device\n");
    return 0;
}

//static methods

static
int32_t
s_enableLayers(const char **layerNames, int32_t layerNamesCount, VkLayerProperties *availableLayers, uint32_t availableLayersCount){
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
int8_t
s_validatePhysicalDevice(VkPhysicalDevice physicalDevice){
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

