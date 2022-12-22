#ifndef __GUM__VULKAN__VULKAN__
#define __GUM__VULKAN__VULKAN__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <stdint.h>

int32_t vulkan_initVulkan(VkInstance *instance, VkDevice *device, VkQueue *graphicsQueue, VkSurfaceKHR *surface, GLFWwindow *window);
int32_t vulkan_createInstance(VkInstance *instance);
int32_t vulkan_selectPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance);
int32_t vulkan_createLogicalDevice(VkDevice *device, VkQueue *graphicsQueue, VkInstance instance, VkPhysicalDevice physicalDevice);

//deletion functions
void vulkan_deleteLogicalDevice(VkDevice *device);
void vulkan_deleteInstance(VkInstance *instance);
void vulkan_deleteVulkan(VkInstance *instance, VkSurfaceKHR *surface, VkDevice *device);



#endif
