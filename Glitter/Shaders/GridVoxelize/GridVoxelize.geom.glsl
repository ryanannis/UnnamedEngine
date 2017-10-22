#version 450 core

layout(triangles) in;

uniform mat4 uMiewMat;
uniform mat4 uModelMat;
uniform mat4 uOrthoProjectionMat;

layout(triangle_strip, max_vertices = 3) out;

void main()
{
	// Project input to clip space

	vec4 csVectors[3];
	
	for(int i = 0; i < 3; i++)
	{
		csVectors[i] = uOrthoProjectionMat * uModelMat * uViewMat;
		csVectors[i] *= csVectors[i] / csVectors[i].w;
	}

	// Emit result
	//temp - passthrough

	for(int i = 0; i < 3; i++)
	{
		glPosition = csVectors[i];
		EmitVertex();
	}

	EmitPrimitive();
}