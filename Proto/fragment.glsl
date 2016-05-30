#version 330 core
in vec4 interpolatedColor;
out vec4 fragmentColor;

void main()
{
	//fragmentColor = interpolatedColor;
	fragmentColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}


