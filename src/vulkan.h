#ifndef __GUM__VULKAN__VULKAN__
#define __GUM__VULKAN__VULKAN__

#include "window.h"


#include <vulkan/vulkan.h>
#include <stdint.h>

//structures

struct vulkan_queueIndices{
    int32_t graphicsQueue;
    int32_t presentQueue;
};

struct vulkan_queueHandles{
    VkQueue graphicsQueue;
    VkQueue presentQueue;
};

struct vulkan_swapchainDetails{
    VkFormat imageFormat;
    VkExtent2D extent;
};

struct vulkan_imageDetails{
    VkImage *images;
    int32_t count;
};

struct vulkan_imageViewDetails{
    VkImageView *imageViews;
    int32_t count;
};

struct vulkan_graphicsStruct{
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    
    struct vulkan_queueIndices queueIndices;
    struct vulkan_queueHandles queueHandles;
    VkSwapchainKHR swapchain;
    struct vulkan_swapchainDetails swapchainDetails;
    
    struct vulkan_imageDetails imageArray;
    struct vulkan_imageViewDetails imageViewArray;
    
};

//function prototypes

int32_t vulkan_initVulkan(struct vulkan_graphicsStruct *graphicsPacket, struct window_window *window);
int32_t vulkan_createInstance(VkInstance *instance);
int32_t vulkan_createSurface(VkSurfaceKHR *surface, VkInstance instance, struct window_window *window);
int32_t vulkan_selectPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, VkSurfaceKHR surface);
int32_t vulkan_createDevice(VkDevice *device, struct vulkan_queueIndices *queueIndices, struct vulkan_queueHandles *queueHandles, VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
int32_t vulkan_createSwapchain(VkSwapchainKHR *swapchain, struct vulkan_swapchainDetails *swapchainDetails, struct vulkan_imageDetails *imageDetails, struct window_window *window, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, struct vulkan_queueIndices *queueIndices, VkDevice device);
int32_t vulkan_createImageViews(struct vulkan_imageViewDetails *imageViewArray, const struct vulkan_imageDetails *imageArray, VkDevice device, const VkFormat *swapchainImageFormat);

//deletion functions
void vulkan_deleteImageViews(struct vulkan_imageViewDetails *restrict imageViewArray, VkDevice device);
void vulkan_deleteSwapchain(VkSwapchainKHR *swapchain, struct vulkan_imageDetails *imageArray, VkDevice device);
void vulkan_deleteDevice(VkDevice *device);
void vulkan_deleteSurface(VkSurfaceKHR *surface, VkInstance instance);
void vulkan_deleteInstance(VkInstance *instance);
void vulkan_destroyVulkan(struct vulkan_graphicsStruct *graphicsPacket);

#endif
