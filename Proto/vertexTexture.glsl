#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

out vec2 interpolatedUV;
uniform mat4 mvpMatrix;
uniform vec2 animKey;

void main()
{
	gl_Position = mvpMatrix * vec4(pos, 1.0f);
	interpolatedUV = uv + animKey;	
}

