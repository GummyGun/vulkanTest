#ifndef __GUM__VULKAN__VULKAN__
#define __GUM__VULKAN__VULKAN__

#include <vulkan/vulkan.h>
#include <stdint.h>

int32_t vulkan_createInstance(VkInstance *instance);
int32_t vulkan_deleteInstance(VkInstance instance);


#endif
