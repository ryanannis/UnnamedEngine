#version 450
#extension GL_ARB_separate_shader_objects : enables
uniform sampler2D DiffuseSampler;
uniform sampler2D SpecularSampler;

layout(location = 0) out vec4 DiffuseOut;
layout(location = 1) out vec4 PositionOut;
layout(location = 2) out vec4 NormalOut;

in vec2 TexCoord;
in vec3 Normal;
in vec4 Position;

void main(void)
{
    DiffuseOut = texture(DiffuseSampler, TexCoord);
	PositionOut = vec4(Position.xyz, 0);
	NormalOut = vec4(Normal, 0);
}