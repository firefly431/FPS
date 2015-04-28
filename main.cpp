#include <iostream>
#include <stdexcept>

#include "TopDownRenderer.h"

int main() {
    try {
        TopDownRenderer renderer(640, 480);
        //renderer.mainloop();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
