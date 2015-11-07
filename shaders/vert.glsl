#version 330
in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 vertex_texture;

out vec3 color;
out vec2 texture_coords;

uniform vec3 vertex_color;
uniform mat4 view, proj;
uniform sampler2D basic_texture;

void main () {
    texture_coords = vertex_texture;
    vec3 light_position = vec3(10.0, 10.0, -5.0);
    vec3 L = normalize(light_position - vertex_position);
    float dot_prod = max(dot(L, vertex_normal), 0.3);
    color = dot_prod * vertex_color;
    gl_Position = proj * view * vec4 (vertex_position, 1.0);
}
