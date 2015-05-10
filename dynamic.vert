#version 150

uniform mat4 model; // must be orthogonal
uniform mat4 view; // assume view matrix is orthogonal
uniform mat4 projection;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out VertexData {
    vec3 normal;
    vec4 eye;
    vec2 texcoord;
} vertex_out;

void main() {
    vec4 p = vec4(position, 1.0);
    vertex_out.texcoord = texcoord;
    vertex_out.eye = -(view * model * p);
    vertex_out.normal = normalize(mat3(view * model) * normal);
    gl_Position = projection * view * model * p;
}
