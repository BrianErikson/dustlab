#version 400
in vec2 text_coords;
out vec4 out_color;

uniform sampler2D image;
uniform vec3 sprite_color;

void main(void)
{
    out_color = vec4(sprite_color, 1.0) * texture(image, text_coords);
}