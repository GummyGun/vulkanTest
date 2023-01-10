#include "utils.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

/*------------------defines------------------*/

/*------------------  enums  ------------------*/

/*------------------  stucts  ------------------*/

/*------------------prototipes------------------*/

//int32_t utils_openFile(struct utils_file *fileData, const char *fileName);
//int32_t utils_closeFile(struct utils_file *file);

//statics

/*------------------    globals    ------------------*/

/*------------------implementations------------------*/

int32_t
utils_openFile(struct utils_file *file, const char *fileName){
    int32_t fileFd;
    if((fileFd = open(fileName, O_RDONLY)) == -1){
        perror("Error: opening file");
        return 1;
    }
    struct stat fileStat;
    fstat(fileFd, &fileStat);
    file->size = fileStat.st_size;
    printf("file: %s size %d\n", fileName, fileStat.st_size);
    file->content = mmap(NULL, file->size, PROT_READ, MAP_PRIVATE, fileFd, 0);
    if(file->content == MAP_FAILED){
        perror("Error: mapping memmory");
        return 1;
    }
    close(fileFd);
    return 0;
}

int32_t
utils_closeFile(struct utils_file *file){
    if(munmap(file->content, file->size) == -1){
        perror("Error: closing file");
        return 1;
    }
    *file = (struct utils_file){};
    return 0;
}

int32_t
utils_syncFile(struct utils_file *file){
    
    return 0;
}
