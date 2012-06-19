#version 330 core

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Color;
smooth out vec4 ex_Color;
uniform mat4 MVP;

void main(void)
{
   // Output position of the vertex, in clip space : MVP * position
   gl_Position = MVP * in_Position;
   ex_Color = in_Color;
}
