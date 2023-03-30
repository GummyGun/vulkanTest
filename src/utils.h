#ifndef __GUM__VULKAN__UTILS__
#define __GUM__VULKAN__UTILS__

#include <stdint.h>

//structures

struct utils_file{
    uint8_t *content;
    int32_t size;
};

struct utils_pointerArray{
    int32_t size;
    void *data;
};

//functions

int32_t utils_openFile(struct utils_file *fileData, const char *fileName);
int32_t utils_closeFile(struct utils_file *file);

int32_t utils_createPArray(struct utils_pointerArray *array, int32_t size);
int32_t utils_deletePArray(struct utils_pointerArray *array);


#endif
