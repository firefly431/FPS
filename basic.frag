#version 150

uniform sampler2D tex;

in VertexData {
    vec3 normal;
    vec2 texcoord;
} vertex_out;

out vec4 color_out;

void main() {
    color_out = vec4(1.0, 0.0, 0.0, 1.0);
}
