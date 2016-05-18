#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include <Renderer.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/transform.hpp>
#include "Player.hpp"

class Application
{
public:
	Application();
	~Application();

	void run();

	void setupGraphic();
	
	void startLoop();

	void grabInput();
	void calculateLogic(double deltaTime);
	void renderScene();

private:
	// Game logic
	Player mPlayer;

	// Rendering stuff
	GLFWwindow *mWnd;
	Renderer mRenderer;
	Shader mShaderColor;
	Shader mShaderTexture;
	GLuint mSampler;

	Texture mBackgroundTex;
	Texture mParalaxBackgroundTex;
	Texture mRampTex;
	Texture mPlayerTex;

	glm::mat4 mViewMatrix;
	glm::mat4 mProjMatrix;
	glm::vec3 mCameraPos;

	GLuint mColoredVao;
	GLuint mTexturedVao;

	GLuint mGizmoVao;
	void setupGizmo();
};

#endif // APPLICATION_HPP

