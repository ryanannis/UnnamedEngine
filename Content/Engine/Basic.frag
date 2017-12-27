#version 450
#extension GL_ARB_separate_shader_objects : enables
out vec4 FragColor;

uniform sampler2D DiffuseSampler;
uniform sampler2D SpecularSampler;


in vec2 TexCoord;

void main()
{
    FragColor = texture(DiffuseSampler, TexCoord);
}
