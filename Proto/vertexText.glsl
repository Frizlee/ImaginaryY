#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;

out vec2 interpolatedUV;
uniform mat4 orthoMatrix;

void main()
{
	gl_Position = orthoMatrix * vec4(pos, 0.0f, 1.0f);
	interpolatedUV = uv;	
}

