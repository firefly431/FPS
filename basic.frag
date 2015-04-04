#version 150

uniform sampler2D tex;

in VertexData {
    vec3 normal;
    vec2 texcoord;
} vertex_out;

out vec4 color_out;

void main() {
    vec3 light_dir = vec3(1.0, 1.0, 1.0);
    vec3 white = vec3(1.0, 1.0, 1.0);
    color_out = vec4(white * dot(vertex_out.normal, light_dir), 1.0);
}
