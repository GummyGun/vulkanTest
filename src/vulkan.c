#include "vulkan.h"
#include "window.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*    .h     */

//int32_t vulkan_createInstance(VkInstance *instance);
//int32_t vulkan_deleteInstance(VkInstance instance);

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
    createInfo.enabledLayerCount = 0;
    
    uint32_t extentionCount = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extentionCount, NULL);
    
    VkExtensionProperties *extensionInfoArray = malloc(extentionCount*sizeof(VkExtensionProperties));
    
    vkEnumerateInstanceExtensionProperties(NULL, &extentionCount, extensionInfoArray);
    
    printf("All extentions\n");
    for(int32_t iter=0; iter<extentionCount; iter++){
        printf("\t%s\n", (extensionInfoArray+iter)->extensionName);
        
    }
    
    free(extensionInfoArray);
    
    if((result = vkCreateInstance(&createInfo, NULL, instance)) != VK_SUCCESS){
        fprintf(stderr,"%d\n", result);
        return 1;
    }
    
    return 0;
}

int32_t 
vulkan_deleteInstance(VkInstance instance){
    vkDestroyInstance(instance, NULL);
    return 0;
}
