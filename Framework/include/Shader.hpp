#ifndef SHADER_HPP
#define SHADER_HPP
#include "gl_core_3_3.hpp"
#include <string>

class Shader
{
	friend class Renderer;

public:
	Shader();

	Shader(std::string vertexShader, std::string fragmentShader);
	void create(std::string vertexShader, std::string fragmentShader);

	~Shader();
	GLint getLocation(std::string name);

	
private:
	GLuint mID;
};

#endif // SHADER_HPP

