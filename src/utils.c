#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
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

/* ================================= extern methods ================================== */
/*                                                                                     */
/*                                                                                     */
/*                _                     __                  _   _                      */
/*               | |                   / _|                | | (_)                     */
/*       _____  _| |_ ___ _ __ _ __   | |_ _   _ _ __   ___| |_ _  ___  _ __  ___      */
/*      / _ \ \/ / __/ _ \ '__| '_ \  |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|     */
/*     |  __/>  <| ||  __/ |  | | | | | | | |_| | | | | (__| |_| | (_) | | | \__ \     */
/*      \___/_/\_\\__\___|_|  |_| |_| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/     */
/*                                                                                     */
/*                                                                                     */
/*                                                                                     */
/* ================================= extern methods ================================== */

int32_t
utils_openFile(struct utils_file *file, const char *fileName){
    int32_t fileFd;
    if((fileFd = open(fileName, O_RDONLY)) == -1){
        perror("Error: Opening file");
        return 1;
    }
    struct stat fileStat;
    fstat(fileFd, &fileStat);
    file->size = fileStat.st_size;
    printf("file: %s size %ld\n", fileName, fileStat.st_size);
    file->content = mmap(NULL, file->size, PROT_READ, MAP_PRIVATE, fileFd, 0);
    if(file->content == MAP_FAILED){
        perror("Error: Mapping memmory");
        return 1;
    }
    close(fileFd);
    return 0;
}

int32_t
utils_closeFile(struct utils_file *file){
    if(munmap(file->content, file->size) == -1){
        perror("Error: Closing file");
        return 1;
    }
    *file = (struct utils_file){};
    return 0;
}

int32_t
utils_syncFile(struct utils_file *file){
    
    return 0;
}

int32_t
utils_createPArray(struct utils_pointerArray *array, int32_t size){
    array->size = size;
    if(!(array->data = calloc(1, sizeof(void*) * size))){
        fprintf(stderr, "Error: Calloc failded\n");
        return 1;
    }
    return 0;
}

int32_t
utils_deletePArray(struct utils_pointerArray *array){
    free(array->data);
    array->data = NULL;
    array->size = 0;
    return 1;
}

/* =============================== static methods =============================== */
/*                                                                                */
/*                                                                                */
/*          _        _   _         __                  _   _                      */
/*         | |      | | (_)       / _|                | | (_)                     */
/*      ___| |_ __ _| |_ _  ___  | |_ _   _ _ __   ___| |_ _  ___  _ __  ___      */
/*     / __| __/ _` | __| |/ __| |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|     */
/*     \__ \ || (_| | |_| | (__  | | | |_| | | | | (__| |_| | (_) | | | \__ \     */
/*     |___/\__\__,_|\__|_|\___| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/     */
/*                                                                                */
/*                                                                                */
/*                                                                                */
/* =============================== static methods =============================== */

