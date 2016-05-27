#ifndef SHADER_HPP
#define SHADER_HPP
#include "Prerequisites.hpp"
#include "GpuResource.hpp"

class Shader : public GpuResource
{
	friend class Renderer;

public:
	Shader();

	// Copy/Move constructors and assignments
	Shader(const Shader &lhs) = delete;
	Shader(Shader &&rhs);
	Shader& operator=(Shader cas) = delete;
	Shader& operator=(Shader &&rhs);

	Shader(std::string vertexShader, std::string fragmentShader);
	void create(std::string vertexShader, std::string fragmentShader);
	
	void clear();
	~Shader();
	GLint getLocation(std::string name);
};

#endif // SHADER_HPP

