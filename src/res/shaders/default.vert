#version 400

layout(location=0) in vec4 vertex; // 2d position, 2d text coords

out vec2 text_coords;

uniform mat4 model;
uniform mat4 projection;

void main(void)
{
    text_coords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}