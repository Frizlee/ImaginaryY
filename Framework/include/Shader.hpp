#ifndef SHADER_HPP
#define SHADER_HPP
#include "Prerequisites.hpp"
#include "GpuResource.hpp"

class Shader : public GpuResource
{
	friend class Renderer;

public:
	Shader();

	Shader(std::string vertexShader, std::string fragmentShader);
	void create(std::string vertexShader, std::string fragmentShader);
	
	void clear();
	~Shader();
	GLint getLocation(std::string name);
};

#endif // SHADER_HPP

