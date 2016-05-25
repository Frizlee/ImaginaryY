#version 330 core
in vec2 interpolatedUV;
layout(location = 0) out vec4 fragmentColor;

uniform sampler2D sampler1;
uniform vec4 textColor;

void main()
{
	fragmentColor = textColor * texture(sampler1, interpolatedUV).x;
	// fragmentColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}

