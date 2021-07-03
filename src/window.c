#include "window.h"
#include <stdlib.h>

bool window_createWindow
(struct window **windowParam, int widthParam, int heightParam, char *nameParam){
    *windowParam=(struct window *)malloc(sizeof(struct window));
    (*windowParam)->width=widthParam;
    (*windowParam)->height=heightParam;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    (*windowParam)->window = glfwCreateWindow(widthParam, heightParam, nameParam, NULL, NULL);
    return 1;
}


bool window_closeWindow(struct window *windowParam){
    return glfwWindowShouldClose(windowParam->window);
}
