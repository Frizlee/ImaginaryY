#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include <Renderer.hpp>
#include <ResourceManager.hpp>
#include <Font.hpp>
#include <Text.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <VertexBuffer.hpp>
#include <VertexArray.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/transform.hpp>
#include "Player.hpp"
#include "Enemy.hpp"

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
	Enemy mEnemy;

	// Rendering stuff
	ResourceManager mResourceManager;
	Font mComicSansFont;
	Text mSimpleText;
	Text mPlayerHP;
	Text mEnemyHP;
	
	std::shared_ptr<VertexBuffer<TextVertexLayout>> mTextBuffer;
	VertexArray mTextArray;
	VertexArray mTextureArray;
	VertexArray mColorArray;
	VertexArray mFullScreenQuadArray;
	VertexArray mGizmoArray;
	std::uint32_t mInfoDisplay;
	std::uint32_t mPlayerDisplay;
	std::uint32_t mEnemyDisplay;

	double mRealDeltaTime;

	GLFWwindow *mWnd;
	Renderer mRenderer;
	Shader mShaderColor;
	Shader mShaderTexture;
	Shader mShaderText;
	GLuint mSampler;

	Texture mBackgroundTex;
	Texture mParalaxBackgroundTex;
	Texture mRampTex;
	Texture mPlayerTex;
	Texture mPlayerAnimTex;
	Texture mEnemyTex;
	Texture mFontTex;
	Texture mTestTex;

	glm::mat4 mOrthoMatrix;
	glm::mat4 mViewMatrix;
	glm::mat4 mProjMatrix;
	glm::vec3 mCameraPos;

	void setupGizmo();
	void setupFullscreenQuad();
};

#endif // APPLICATION_HPP

