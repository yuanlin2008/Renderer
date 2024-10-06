#include "renderer.h"

int main(int argc, char *argv[])
{
    Renderer renderer;
    renderer.init();
    renderer.run();
    renderer.close();
    return 0;
}