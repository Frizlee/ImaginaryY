#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;

out vec4 interpolatedColor;
uniform mat4 mvpMatrix;

void main()
{
	gl_Position = mvpMatrix * vec4(pos, 1.0f);
	interpolatedColor = color;	
}

