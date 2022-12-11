#ifndef __GUM__VULKAN__VULKAN__
#define __GUM__VULKAN__VULKAN__

#include <vulkan/vulkan.h>
#include <stdint.h>

int32_t vulkan_initVulkan(VkInstance *instance, VkPhysicalDevice *physicalDevice);
int32_t vulkan_createInstance(VkInstance *instance);
int32_t vulkan_createPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance);

//deletion functions
int32_t vulkan_deleteVulkan(VkInstance *instance, VkPhysicalDevice *physicalDevice);
int32_t vulkan_deleteInstance(VkInstance *instance);
int32_t vulkan_deletePhysicalDevice(VkPhysicalDevice *physicalDevice);


#endif
