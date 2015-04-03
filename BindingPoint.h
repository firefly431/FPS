#pragma once

class BindingPoint {
private:
    static unsigned int current;
public:
    inline static unsigned int next() {
        return ++current;
    }
};

