#include "Buffer.h"

// explicit instantiation
// required since we're using extern template
template class Buffer<GL_UNIFORM_BUFFER>;
template class Buffer<GL_ARRAY_BUFFER>;
template class Buffer<GL_ELEMENT_ARRAY_BUFFER>;

