#include "Application.hpp"
#include <fstream>
#include <streambuf>
#include <string>
#include <vector>
#include <iostream>
#include <PngCodec.hpp>
#include <ImageAtlas.hpp>
#include <chrono>
#include <VertexLayout.hpp>
#include <Text.hpp>
#include <sstream>
using namespace std;

#define WIDTH 1366
#define HEIGHT 768

// metric dimensions
#define PLAYER_HEIGHT 1.8f
#define PLAYER_WIDTH 1.0f
#define CAMERA_DISTANCE 7.0f

// camera move rectangle
#define RECT_X_L -5.0f 
#define RECT_X_R 5.0f
#define GL_CHECK {							\
GLenum e;									\
if ((e = gl::GetError()) != gl::NO_ERROR_)	\
cout << "glGetError: " << e << endl;		\
}


typedef TextureVertexLayout::Data TVLData;
typedef ColorVertexLayout::Data CVLData;

#pragma pack(push)
struct VertexColor
{
	float x, y, z;
	float r, g, b, a;

	VertexColor(float x, float y, float z, float r, float g, float b, float a) :
		x{ x }, y{ y }, z{ z }, r{ r }, g{ g }, b{ b }, a{ a } {}
};
#pragma pack(pop)

Application::Application()
{
	glfwInit();

	// OpenGl initialization
}

Application::~Application()
{
	glfwTerminate();
}

void Application::run()
{	
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	mWnd = glfwCreateWindow(WIDTH, HEIGHT, "ImaginaryY", nullptr, nullptr);
	glfwMakeContextCurrent(mWnd);
	glfwSwapInterval(0);
	mRenderer.init();

	ResourceHandle<ImageAtlas> cs = mResourceManager.getFreeHandle<ImageAtlas>("ComicSans atlas");
	cs.get()->create(256, 256, Image::Format::R8);

	mComicSansFont.create("ComicSans.ttf", 25, cs);

	setupGraphic();

	mPlayerHP.create("100", mComicSansFont);
	mEnemyHP.create("100", mComicSansFont);

	// Setup matrices and other
	mPlayer.setPos(glm::vec3(0.0f, 0.0f, 0.0f));
	mPlayer.setSpeed(1.5f);
	mEnemy.setStartPos(glm::vec3(2.0f, 0.0f, 2.0f));
	mEnemy.setSpeed(1.0f);
	mCameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	
	mProjMatrix = glm::perspective(
		glm::radians(90.0f), // Fov
		WIDTH / static_cast<float>(HEIGHT), // Aspect ratio
		1.0f, // Near clip
		1000.0f // Far clip		
	);

	mOrthoMatrix = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT));

	startLoop();

	glfwDestroyWindow(mWnd);
}

void Application::setupGraphic()
{
	

	ifstream file;
	Image img;
	string vertexShader, fragmentShader;
	vector<CVLData> buffer1;
	vector<TVLData> buffer2;
	shared_ptr<VertexBuffer<TextureVertexLayout>> textureBuffer;
	shared_ptr<VertexBuffer<ColorVertexLayout>> colorBuffer;

	gl::ClearColor(0.1875f, 0.8359375f, 0.78125f, 1.0f);
	gl::Viewport(0, 0, WIDTH, HEIGHT);
	mRenderer.setDepthTest(true);
	gl::Enable(gl::BLEND);
	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

	// Fill up buffers
	// buffer1.assign({});

	buffer2.assign({
		// Background
		TVLData{ -15.0f, -6.0f, 0.0f, 0.0f, 0.0f },
		TVLData{ -15.0f, 6.0f, 0.0f, 0.0f, 1.0f },
		TVLData{ 15.0f, 6.0f, 0.0f, 1.0f, 1.0f },
		TVLData{ 15.0f, 6.0f, 0.0f, 1.0f, 1.0f },
		TVLData{ 15.0f, -6.0f, 0.0f, 1.0f, 0.0f },
		TVLData{ -15.0f, -6.0f, 0.0f, 0.0f, 0.0f },

		// Paralax background
		TVLData{ -120.0f, -50.0f, -12.0f, 0.0f, 0.0f },
		TVLData{ -120.0f, 50.0f, -12.0f, 0.0f, 1.0f },
		TVLData{ 120.0f, 50.0f, -12.0f, 1.0f, 1.0f },
		TVLData{ 120.0f, 50.0f, -12.0f, 1.0f, 1.0f },
		TVLData{ 120.0f, -50.0f, -12.0f, 1.0f, 0.0f },
		TVLData{ -120.0f, -50.0f, -12.0f, 0.0f, 0.0f },
		
		// Ramp
		TVLData{ -15.0f, 0.0f, 15.0f, 0.0f, 0.0f },
		TVLData{ -15.0f, 0.0f, 0.0f, 0.0f, 1.0f },
		TVLData{ 15.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		TVLData{ 15.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		TVLData{ 15.0f, 0.0f, 15.0f, 1.0f, 0.0f },
		TVLData{ -15.0f, 0.0f, 15.0f, 0.0f, 0.0f },

		// PLayer / Enemy
		TVLData{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		TVLData{ 0.0f, PLAYER_HEIGHT, 0.0f, 0.0f, 0.5f },
		TVLData{ PLAYER_WIDTH, PLAYER_HEIGHT, 0.0f, 0.279296875f, 0.5f },
		TVLData{ PLAYER_WIDTH, PLAYER_HEIGHT, 0.0f, 0.279296875f, 0.5f },
		TVLData{ PLAYER_WIDTH, 0.0f, 0.0f, 0.279296875f, 0.0f },
		TVLData{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
	});

	// Load shaders
	file.open("vertex.glsl");
	vertexShader.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
	file.close();
	file.open("fragment.glsl");
	fragmentShader.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
	file.close();

	mShaderColor.create(vertexShader, fragmentShader);

	file.open("vertexTexture.glsl");
	vertexShader.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
	file.close();
	file.open("fragmentTexture.glsl");
	fragmentShader.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
	file.close();

	mShaderTexture.create(vertexShader, fragmentShader);

	file.open("vertexText.glsl");
	vertexShader.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
	file.close();
	file.open("fragmentText.glsl");
	fragmentShader.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
	file.close();

	mShaderText.create(vertexShader, fragmentShader);

	// Create color vao
	colorBuffer = make_shared<VertexBuffer<ColorVertexLayout>>(buffer1.size() * ColorVertexLayout::Size(), mRenderer);
	mColorArray.create(colorBuffer, mRenderer);

	// Create texture vao
	textureBuffer = make_shared<VertexBuffer<TextureVertexLayout>>(buffer2.size() * TextureVertexLayout::Size(), 
		mRenderer);
	mTextureArray.create(textureBuffer, mRenderer);
	
	// Create text vao
	mTextBuffer = make_shared<VertexBuffer<TextVertexLayout>>(1024 * 1024, mRenderer);
	mTextArray.create(mTextBuffer, mRenderer);

	// Send data
	colorBuffer->add(buffer1, mRenderer);
	textureBuffer->add(buffer2, mRenderer);	

	// Load textures;
	file.open("image.png", ios_base::binary);
	PngCodec().decode(file, img);
	file.close();
	mBackgroundTex.create(img, mRenderer);
	file.open("image1.png", ios_base::binary);
	PngCodec().decode(file, img);
	file.close();
	mParalaxBackgroundTex.create(img, mRenderer);
	file.open("image2.png", ios_base::binary);
	PngCodec().decode(file, img);
	file.close();
	mRampTex.create(img, mRenderer);
	file.open("player1.png", ios_base::binary);
	PngCodec().decode(file, img);
	file.close();
	mPlayerTex.create(img, mRenderer);
	file.open("player.png", ios_base::binary);
	PngCodec().decode(file, img);
	file.close();
	mEnemyTex.create(img, mRenderer);
	file.open("playerAnim.png", ios_base::binary);
	PngCodec().decode(file, img);
	file.close();
	mPlayerAnimTex.create(img, mRenderer);
	mFontTex.create(*mComicSansFont.getAtlas().get(), mRenderer);

	// Test
	mTestTex.create({ &img }, mRenderer);

	// Setup sampler
	gl::GenSamplers(1, &mSampler);
	gl::SamplerParameteri(mSampler, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
	gl::SamplerParameteri(mSampler, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
	gl::SamplerParameteri(mSampler, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
	gl::SamplerParameteri(mSampler, gl::TEXTURE_MAG_FILTER, gl::LINEAR);

	// Setup gizmo
	setupGizmo();

	// Setup fullscreen quad;
	setupFullscreenQuad();
}


void Application::startLoop()
{
	double accumulator = 0.0;
	const double updateTimeStep = 1.0 / 60.0;

	auto previous = chrono::steady_clock::now();
	auto current = chrono::steady_clock::now();
	chrono::duration<double> diff;


	while (!glfwWindowShouldClose(mWnd))
	{
		current = chrono::steady_clock::now();
		diff = current - previous;
		previous = current;
		accumulator += mRealDeltaTime = diff.count();

		grabInput();

		while (accumulator > updateTimeStep)
		{
			accumulator -= updateTimeStep;

			calculateLogic(updateTimeStep);
		}

		renderScene();
	}
}

void Application::grabInput()
{
	glfwPollEvents();

	mPlayer.setMoveUp(glfwGetKey(mWnd, GLFW_KEY_UP) == GLFW_PRESS ? true : false);
	mPlayer.setMoveDown(glfwGetKey(mWnd, GLFW_KEY_DOWN) == GLFW_PRESS ? true : false);
	mPlayer.setMoveLeft(glfwGetKey(mWnd, GLFW_KEY_LEFT) == GLFW_PRESS ? true : false);
	mPlayer.setMoveRight(glfwGetKey(mWnd, GLFW_KEY_RIGHT) == GLFW_PRESS ? true : false);
}

void Application::calculateLogic(double deltaTime)
{
	mViewMatrix = glm::lookAt(
		glm::vec3(0.0f, 3.0f, CAMERA_DISTANCE) + mCameraPos, // Camera position
		glm::vec3(0.0f, 3.0f /*PLAYER_HEIGHT*/, 0.0f) + mCameraPos, // Look at the player head
		glm::vec3(0.0f, 1.0f, 0.0f) // Up
		);

	if (glm::length(mPlayer.getPos() - mEnemy.getPos()) < 2.0f)
		mEnemy.setMoveDir(glm::normalize(mPlayer.getPos() - mEnemy.getPos()));
	else
	{
		if (glm::length(mEnemy.getPos() - mEnemy.getStartPos()) < 0.1f)
			mEnemy.setMoveDir(glm::vec3(0.0f, 0.0f, 0.0f));
		else
			mEnemy.setMoveDir(glm::normalize(mEnemy.getStartPos() - mEnemy.getPos()));
	}

	mPlayer.update(deltaTime);
	mEnemy.update(deltaTime);

	stringstream infoDisplay;
	infoDisplay << endl;
	infoDisplay << "dt: " << mRealDeltaTime << endl;
	infoDisplay << "PlayerX: " << mPlayer.getPos().x << endl;
	infoDisplay << "PlayerZ: " << mPlayer.getPos().z << endl;
	infoDisplay << "EnemyX: " << mEnemy.getPos().x << endl;
	infoDisplay << "EnemyZ: " << mEnemy.getPos().z << endl;

	mSimpleText.create(infoDisplay.str(), mComicSansFont);
	mTextBuffer->clear();
	mInfoDisplay = mTextBuffer->add(mSimpleText.getBuffer(), mRenderer);
	mPlayerDisplay = mTextBuffer->add(mPlayerHP.getBuffer(), mRenderer);
	mEnemyDisplay = mTextBuffer->add(mEnemyHP.getBuffer(), mRenderer);
}

void Application::renderScene()
{
	static GLint samplerLocationTexture = mShaderTexture.getLocation("sampler1");
	static GLint matrixLocationTexture = mShaderTexture.getLocation("mvpMatrix");
	static GLint animKeyLocationTexture = mShaderTexture.getLocation("animKey");
	static GLint matrixLocationColor = mShaderColor.getLocation("mvpMatrix");
	static GLint orthoMatrixLocationText = mShaderText.getLocation("orthoMatrix");
	static GLint samplerLocationText = mShaderText.getLocation("sampler1");
	static GLint colorLocationText = mShaderText.getLocation("textColor");
	
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Render colored
	mRenderer.bindShader(mShaderColor);

	mRenderer.bindVertexArray(mGizmoArray);
	
	gl::UniformMatrix4fv(matrixLocationColor, 1, gl::FALSE_,
		&(mProjMatrix * mViewMatrix * glm::translate(mPlayer.getPos()))[0][0]);
	
	gl::DrawArrays(gl::LINES, 0, 6);

	// Render texutred
	mRenderer.bindShader(mShaderTexture);

	mRenderer.bindVertexArray(mTextureArray);
	
	gl::Uniform2fv(animKeyLocationTexture, 1, &glm::vec2(0.0f, 0.0f)[0]);
	gl::UniformMatrix4fv(matrixLocationTexture, 1, gl::FALSE_, &(mProjMatrix * mViewMatrix)[0][0]);
	gl::Uniform1i(samplerLocationTexture, 0);
	gl::BindSampler(0, mSampler);
	
	mRenderer.bindTexture(mParalaxBackgroundTex, 0);
	gl::DrawArrays(gl::TRIANGLES, 6, 6);

	mRenderer.bindTexture(mBackgroundTex, 0);
	gl::DrawArrays(gl::TRIANGLES, 0, 6);

	mRenderer.bindTexture(mRampTex, 0);
	gl::DrawArrays(gl::TRIANGLES, 12, 6);
	
	// Enemy
	gl::UniformMatrix4fv(matrixLocationTexture, 1, gl::FALSE_,
		&(mProjMatrix * mViewMatrix * glm::translate(mEnemy.getPos()))[0][0]);
	
	mRenderer.bindTexture(mPlayerAnimTex, 0);
	gl::DrawArrays(gl::TRIANGLES, 18, 6);

	// Player
	gl::Uniform2fv(animKeyLocationTexture, 1, &mPlayer.getAnimKey()[0]);
	gl::UniformMatrix4fv(matrixLocationTexture, 1, gl::FALSE_,
		&(mProjMatrix * mViewMatrix * glm::translate(mPlayer.getPos()))[0][0]);
	
	mRenderer.bindTexture(mPlayerAnimTex, 0);
	//mRenderer.bindTexture(mTestTex, 0);
	gl::DrawArrays(gl::TRIANGLES, 18, 6);
	
	mRenderer.bindVertexArray(mFullScreenQuadArray);
	gl::DrawArrays(gl::TRIANGLES, 0, 6);

	// Render text
	mRenderer.bindShader(mShaderText);

	mRenderer.bindVertexArray(mTextArray);

	gl::Uniform4fv(colorLocationText, 1, &glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)[0]);
	gl::UniformMatrix4fv(orthoMatrixLocationText, 1, gl::FALSE_,
		&(mOrthoMatrix * glm::translate(glm::vec3(0.0f, static_cast<float>(HEIGHT), 0.0f)))[0][0]);
	// gl::UniformMatrix4fv(orthoMatrixLocationText, 1, gl::FALSE_, &glm::mat4()[0][0]);
	gl::Uniform1i(samplerLocationText, 0);
	gl::BindSampler(0, mSampler);
	
	mRenderer.bindTexture(mFontTex, 0);
	gl::DrawArrays(gl::TRIANGLES, mInfoDisplay / TextVertexLayout::Size(), 
		mSimpleText.getBuffer().size());

	// Calculate player pos on 2d space
	glm::vec4 transVec = mProjMatrix * mViewMatrix * glm::vec4(mPlayer.getPos(), 1.0f);
	transVec.x /= transVec.w;
	transVec.y /= transVec.w;
	transVec.x = WIDTH * (transVec.x + 1.0f) / 2.0f;
	transVec.y = HEIGHT * (transVec.y + 1.0f) / 2.0f;
	transVec.z = 0.0f;
	transVec.w = 1.0f;

	gl::UniformMatrix4fv(orthoMatrixLocationText, 1, gl::FALSE_,
		&(mOrthoMatrix * glm::translate(glm::vec3(transVec)))[0][0]);
	
	gl::DrawArrays(gl::TRIANGLES, mPlayerDisplay / TextVertexLayout::Size(), 
		mPlayerHP.getBuffer().size());

	// Calculate Enemy pos on 2d space
	transVec = mProjMatrix * mViewMatrix * glm::vec4(mEnemy.getPos(), 1.0f);
	transVec.x /= transVec.w;
	transVec.y /= transVec.w;
	transVec.x = WIDTH * (transVec.x + 1.0f) / 2.0f;
	transVec.y = HEIGHT * (transVec.y + 1.0f) / 2.0f;
	transVec.z = 0.0f;
	transVec.w = 1.0f;

	gl::UniformMatrix4fv(orthoMatrixLocationText, 1, gl::FALSE_,
	 	&(mOrthoMatrix * glm::translate(glm::vec3(transVec)))[0][0]);
	
	gl::DrawArrays(gl::TRIANGLES, mEnemyDisplay / TextVertexLayout::Size(), 
		mEnemyHP.getBuffer().size());

	glfwSwapBuffers(mWnd);

	GL_CHECK;
}

void Application::setupGizmo()
{
	vector<CVLData> buffer;
	shared_ptr<VertexBuffer<ColorVertexLayout>> vbo;

	// Fill up buffer
	buffer.assign({
		CVLData{ 0.0f, 0.0f, 0.0f, 0xff0000ff },
		CVLData{ 1.0f, 0.0f, 0.0f, 0xff0000ff },
		CVLData{ 0.0f, 0.0f, 0.0f, 0x00ff00ff },
		CVLData{ 0.0f, 1.0f, 0.0f, 0x00ff00ff },
		CVLData{ 0.0f, 0.0f, 0.0f, 0x0000ffff },
		CVLData{ 0.0f, 0.0f, 1.0f, 0x0000ffff },
	});

	vbo = make_shared<VertexBuffer<ColorVertexLayout>>(buffer.size() * ColorVertexLayout::Size(), mRenderer);
	mGizmoArray.create(vbo, mRenderer);
}

void Application::setupFullscreenQuad()
{
	vector<TVLData> bufferData;
	shared_ptr<VertexBuffer<TextureVertexLayout>> vbo;

	// Fill up buffer
	bufferData.assign({
		TVLData{ -1.0f, -1.0f, 0.0f, 0.0f, 0.0f },
		TVLData{ -1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		TVLData{ 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
		TVLData{ 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
		TVLData{ 1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
		TVLData{ -1.0f, -1.0f, 0.0f, 0.0f, 0.0f },
	});

	vbo = make_shared<VertexBuffer<TextureVertexLayout>>(bufferData.size() * TextureVertexLayout::Size(), mRenderer);
	mFullScreenQuadArray.create(vbo, mRenderer);
}

