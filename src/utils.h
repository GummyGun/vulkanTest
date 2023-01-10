#ifndef __GUM__VULKAN__UTILS__
#define __GUM__VULKAN__UTILS__

#include <stdint.h>

//structures

struct utils_file{
    uint8_t *content;
    int32_t size;
};

//functions

int32_t utils_openFile(struct utils_file *fileData, const char *fileName);
int32_t utils_closeFile(struct utils_file *file);



#endif
