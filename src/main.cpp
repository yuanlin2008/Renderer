#include "GLFW/glfw3.h"
#include <stdexcept>
#include "rhi.h"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    RHI rhi;
    rhi.init();

    auto window = glfwCreateWindow(800, 600, "Renderer", nullptr, nullptr);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
    rhi.close();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}