#!/bin/bash

/home/gumgun/VulkanSDK/installDebug/bin/glslc "$1/$2/"shader.frag -o "$1/$2/"shader.frag.spv
/home/gumgun/VulkanSDK/installDebug/bin/glslc "$1/$2/"shader.vert -o "$1/$2/"shader.vert.spv


