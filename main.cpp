#include <iostream>
#include <stdexcept>

#include "GLFWRenderer.h"

int main() {
    try {
        GLFWRenderer renderer(640, 480);
        renderer.mainloop();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
