#!/bin/bash

parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

cd "$parent_path"

wayland-scanner private-code < /home/nxf95416/imx/71/sysroots/armv8a-poky-linux/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml > ../res/wayland/xdg-shell-protocol.c
wayland-scanner client-header < /home/nxf95416/imx/71/sysroots/armv8a-poky-linux/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml > ../res/wayland/xdg-shell-client-protocol.h
#wayland-scanner private-code < $1 > xdg-shell-protocol.c
#wayland-scanner client-header < $1 > xdg-shell-client-protocol.h



