#include <iostream>
#include <stdexcept>

#include "SFMLRenderer.h"

int main() {
    try {
        SFMLRenderer renderer(640, 480);
        renderer.mainloop();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
