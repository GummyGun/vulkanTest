#ifndef __GUM__VULKAN__VULKAN__
#define __GUM__VULKAN__VULKAN__

#include "window.h"


#include <vulkan/vulkan.h>
#include <stdint.h>

//structures

struct vInit_queueIndices{
    int32_t graphicsQueue;
    int32_t presentQueue;
};

struct vInit_queueHandles{
    VkQueue graphicsQueue;
    VkQueue presentQueue;
};

struct vInit_swapchainDetails{
    VkFormat imageFormat;
    VkExtent2D extent;
};

struct vInit_imageDetails{
    VkImage *images;
    int32_t count;
};

struct vInit_imageViewDetails{
    VkImageView *imageViews;
    int32_t count;
};

struct vInit_graphicsStruct{
    int32_t debugMode;
    
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    
    struct vInit_queueIndices queueIndices;
    struct vInit_queueHandles queueHandles;
    VkSwapchainKHR swapchain;
    struct vInit_swapchainDetails swapchainDetails;
    
    struct vInit_imageDetails imageArray;
    struct vInit_imageViewDetails imageViewArray;
    
};

//function prototypes

int32_t vInit_initVulkan(struct vInit_graphicsStruct *graphicsPacket, struct window_window *window);
int32_t vInit_createInstance(VkInstance *instance, int32_t debugMode);
int32_t vInit_createDebugMessenger(VkDebugUtilsMessengerEXT *debugMessenger, int32_t debugMode, VkInstance instance);
int32_t vInit_createSurface(VkSurfaceKHR *surface, VkInstance instance, struct window_window *window);
int32_t vInit_selectPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, VkSurfaceKHR surface);
int32_t vInit_createDevice(VkDevice *device, struct vInit_queueIndices *queueIndices, struct vInit_queueHandles *queueHandles, VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
int32_t vInit_createSwapchain(VkSwapchainKHR *swapchain, struct vInit_swapchainDetails *swapchainDetails, struct vInit_imageDetails *imageDetails, struct window_window *window, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, struct vInit_queueIndices *queueIndices, VkDevice device);
int32_t vInit_createImageViews(struct vInit_imageViewDetails *imageViewArray, const struct vInit_imageDetails *imageArray, VkDevice device, const VkFormat *swapchainImageFormat);

//deletion functions
void vInit_deleteImageViews(struct vInit_imageViewDetails *restrict imageViewArray, VkDevice device);
void vInit_deleteSwapchain(VkSwapchainKHR *swapchain, struct vInit_imageDetails *imageArray, VkDevice device);
void vInit_deleteDevice(VkDevice *device);
void vInit_deleteSurface(VkSurfaceKHR *surface, VkInstance instance);
void vInit_deleteDebugMessenger(VkDebugUtilsMessengerEXT *debugMessenger, VkInstance instance);
void vInit_deleteInstance(VkInstance *instance);
void vInit_destroyVulkan(struct vInit_graphicsStruct *graphicsPacket);

#endif
