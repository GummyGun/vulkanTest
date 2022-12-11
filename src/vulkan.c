#include "vulkan.h"
#include "window.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------defines------------------*/
#define VULKAN_MAX_LAYER_NAME 64

/*------------------prototipes------------------*/

//int32_t vulkan_createInstance(VkInstance *instance);
//int32_t vulkan_deleteInstance(VkInstance instance);
static int32_t s_enableLayers(const char **layerNames, int32_t layerNamesCount, VkLayerProperties *availableLayers, uint32_t availableLayersCount);
static void s_getExtensions(VkExtensionProperties **extensions, uint32_t *extensionsCount);
static void s_getLayers(VkLayerProperties **layers, uint32_t *layersCount);
static void s_freeExtensions(VkExtensionProperties **extensions);
static void s_freeLayers(VkLayerProperties **layers);

/*------------------implementations------------------*/

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
        printf("\t%s\n", glfwExtensions[iter]);
        
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
vulkan_deleteInstance(VkInstance instance){
    vkDestroyInstance(instance, NULL);
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
            if(!strcmp(*(layerNames+(iter)), availableLayers[iterAvLay].layerName)){
                found=1;
            }
        }
        if(!found){
            printf("badret\n");
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
s_getLayers(VkLayerProperties **layers, uint32_t *layersCount){
    vkEnumerateInstanceLayerProperties(layersCount, NULL);
    *layers = malloc((*layersCount)*sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(layersCount, *layers);
}

static 
void 
s_freeExtensions(VkExtensionProperties **extensions){
    free(*extensions);
    *extensions=NULL;
}

static 
void 
s_freeLayers(VkLayerProperties **layers){
    free(*layers);
    *layers=NULL;
}

