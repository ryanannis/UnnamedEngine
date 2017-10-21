#version 440 core

layout (location = 0) in vec4 a_vert;

void main()
{
	gl_Position = a_vert;
}