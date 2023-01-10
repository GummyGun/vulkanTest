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
    struct vulkan_queueHandles queuesHandles;
    VkSwapchainKHR swapchain;
    struct vulkan_swapchainDetails swapchainDetails;
    
    struct vulkan_imageDetails imageArray;
    struct vulkan_imageViewDetails imageViewArray;
};

//function prototypes

int32_t vulkan_initVulkan(struct vulkan_graphicsStruct *graphicsPacket, GLFWwindow *window);
int32_t vulkan_createInstance(VkInstance *instance);
int32_t vulkan_createSurface(VkSurfaceKHR *surface, VkInstance instance, GLFWwindow *window);
int32_t vulkan_selectPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, VkSurfaceKHR surface);
int32_t vulkan_createLogicalDevice(VkDevice *device, struct vulkan_queueHandles *queueHandles, VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
int32_t vulkan_createSwapchain(VkSwapchainKHR *swapchain, struct vulkan_imageDetails *imageDetails, struct vulkan_swapchainDetails *swapchainDetails, GLFWwindow *window, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkDevice device);
int32_t vulkan_createImageViews(struct vulkan_imageViewDetails *restrict imageViewArray, const struct vulkan_imageDetails *const restrict imageArray, restrict VkDevice device, const VkFormat *restrict const swapchainImageFormat);

//deletion functions
void vulkan_deleteImageViews(struct vulkan_imageViewDetails *restrict imageViewArray, VkDevice device);
void vulkan_deleteSwapchain(VkSwapchainKHR *swapchain, struct vulkan_imageDetails *imageArray, VkDevice device);
void vulkan_deleteLogicalDevice(VkDevice *device);
void vulkan_deleteSurface(VkSurfaceKHR *surface, VkInstance instance);
void vulkan_deleteInstance(VkInstance *instance);
void vulkan_deleteVulkan(struct vulkan_graphicsStruct *graphicsPacket);


#endif
