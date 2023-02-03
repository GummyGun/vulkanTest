#!/bin/bash
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"

ssh root@$TESTIP 'mkdir -p res/shaders'
scp ../res/shaders/*.spv root@$TESTIP:~/res/shaders
