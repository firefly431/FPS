#version 150

layout (std140) uniform view_projection {
    mat4 view; // assume view matrix is orthogonal
    mat4 projection;
};

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out VertexData {
    vec3 normal;
    vec2 texcoord;
} vertex_out;

void main() {
    vertex_out.texcoord = texcoord;
    vertex_out.normal = normalize(mat3(view) * normal);
    gl_Position = projection * view * vec4(position, 1.0);
}
