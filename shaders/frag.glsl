#version 330
in vec3 color;
in vec2 texture_coords;

out vec4 frag_colour;

uniform sampler2D basic_texture;

void main () {
    vec4 texel = texture(basic_texture, texture_coords);
    frag_colour = vec4(color, 1.0) + 0.4 * texel;
}
