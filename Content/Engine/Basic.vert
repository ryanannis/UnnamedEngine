#version 450
#extension GL_ARB_separate_shader_objects : enable

//layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aNormal;
//layout(location = 2) in vec2 aTexCoord;

layout(location = 0) out vec2 TexCoord;

//layout(binding = 0) uniform UniformBufferObject  {
//    mat4 MVP;
//} ubo;

void main()
{
    //gl_Position = ubo.MVP * vec4(aPos, 1.0);
    //TexCoord = aTexCoord;
}