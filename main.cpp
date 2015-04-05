#include <iostream>
#include <stdexcept>

#include "OpenGLRenderer.h"

int main() {
    try {
        OpenGLRenderer renderer(640, 480);
        renderer.mainloop();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
