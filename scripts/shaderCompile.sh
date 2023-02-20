#!/bin/bash
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"

/home/gumgun/VulkanSDK/installDebug/bin/glslc ../res/shaders/shader.frag -o ../res/shaders/shader.frag.spv
/home/gumgun/VulkanSDK/installDebug/bin/glslc ../res/shaders/shader.vert -o ../res/shaders/shader.vert.spv


