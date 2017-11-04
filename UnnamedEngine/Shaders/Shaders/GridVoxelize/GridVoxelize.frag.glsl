#version 450 core

layout (binding = 0) uniform writeonly uimage3D output;

out vec4 color;	

void main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0);
}