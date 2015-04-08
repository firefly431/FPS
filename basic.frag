#version 150

uniform sampler2D tex;

in VertexData {
    vec3 normal;
    vec4 eye;
    vec2 texcoord;
} vertex_out;

out vec4 color_out;

void main() {
	/*
    vec4 diffuse = texture(tex, vertex_out.texcoord);
    vec4 specular = vec4(1.0);
    float shininess = 128.0;
    vec3 light_dir = normalize(vec3(1.0, 1.0, 1.0));
    float ambient = 0.2;
    vec4 spec = vec4(0.0);
    vec3 n = normalize(vertex_out.normal);
    vec3 e = normalize(vec3(vertex_out.eye));
    float intensity = max(dot(n, light_dir), ambient);
    if (intensity > 0.0) {
        vec3 h = normalize(light_dir + e);
        float i = max(dot(h, n), 0.0);
        spec = specular * pow(i, shininess);
    }
    color_out = vec4(diffuse * intensity + spec);
	*/
	color_out = vec4(1.0);
}
