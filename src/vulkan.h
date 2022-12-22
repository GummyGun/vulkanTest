#ifndef __GUM__VULKAN__VULKAN__
#define __GUM__VULKAN__VULKAN__

#include <vulkan/vulkan.h>
#include <stdint.h>

int32_t vulkan_initVulkan(VkInstance *instance, VkDevice *device, VkQueue *graphicsQueue);
int32_t vulkan_createInstance(VkInstance *instance);
int32_t vulkan_selectPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance);
int32_t vulkan_createLogicalDevice(VkDevice *device, VkQueue *graphicsQueue, VkInstance instance, VkPhysicalDevice physicalDevice);

//deletion functions
int32_t vulkan_deleteVulkan(VkInstance *instance, VkDevice *device);
int32_t vulkan_deleteInstance(VkInstance *instance);
void vulkan_deleteLogicalDevice(VkDevice *device);



#endif
