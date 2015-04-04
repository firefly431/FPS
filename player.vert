#version 150

uniform mat4 view; // assume view matrix is orthogonal
uniform mat4 projection;

uniform mat4 model; // must be orthogonal

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out VertexData {
    vec3 normal;
    vec2 texcoord;
} vertex_out;

void main() {
    vertex_out.texcoord = texcoord;
    vertex_out.normal = normalize(mat3(view * model) * normal);
    gl_Position = projection * view * model * vec4(position, 1.0);
}
