#version 330 core
in vec2 interpolatedUV;
layout(location = 0) out vec4 fragmentColor;

uniform sampler2D sampler1;

void main()
{
	fragmentColor = texture(sampler1, interpolatedUV);
	if (fragmentColor.a < 0.1)
		discard;
}


