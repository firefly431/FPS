#include <iostream>
#include <stdexcept>

#include "OpenGLRenderer.h"

int main() {
    try {
        OpenGLRenderer renderer(640, 480);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
