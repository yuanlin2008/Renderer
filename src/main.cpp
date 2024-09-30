#include "GLFW/glfw3.h"
#include "volk.h"

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    volkInitialize();

    auto window = glfwCreateWindow(800,600, "Renderer", nullptr, nullptr);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}