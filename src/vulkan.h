#ifndef __GUM__VULKAN__VULKAN__
#define __GUM__VULKAN__VULKAN__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <stdint.h>

//structures

struct vulkan_queueHandles{
    VkQueue graphicsQueue;
    VkQueue presentQueue;
};

struct vulkan_graphicsStruct{
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    struct vulkan_queueHandles queuesHandles;
};

//function prototypes

int32_t vulkan_initVulkan(struct vulkan_graphicsStruct *graphicsPacket, GLFWwindow *window);
int32_t vulkan_createInstance(VkInstance *instance);
int32_t vulkan_createSurface(VkSurfaceKHR *surface, VkInstance instance, GLFWwindow *window);
int32_t vulkan_selectPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, VkSurfaceKHR surface);
int32_t vulkan_createLogicalDevice(VkDevice *device, struct vulkan_queueHandles *queueHandles, VkInstance instance, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

//deletion functions
void vulkan_deleteLogicalDevice(VkDevice *device);
void vulkan_deleteSurface(VkInstance *instance, VkSurfaceKHR *surface);
void vulkan_deleteInstance(VkInstance *instance);
void vulkan_deleteVulkan(struct vulkan_graphicsStruct *graphicsPacket);



#endif
