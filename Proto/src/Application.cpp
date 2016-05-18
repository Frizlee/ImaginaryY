#include "Application.hpp"
#include <fstream>
#include <streambuf>
#include <string>
#include <vector>
#include <iostream>
#include <PngCodec.hpp>
#include <ImageAtlas.hpp>
#include <chrono>
using namespace std;

#define WIDTH 1920
#define HEIGHT 1080

// metric dimensions
#define PLAYER_HEIGHT 1.8f
#define PLAYER_WIDTH 1.0f
#define CAMERA_DISTANCE 7.0f

// camera move rectangle
#define RECT_X_L -5.0f 
#define RECT_X_R 5.0f

#pragma pack(push)
struct VertexColor
{
	float x, y, z;
	float r, g, b, a;

	VertexColor(float x, float y, float z, float r, float g, float b, float a) :
		x{ x }, y{ y }, z{ z }, r{ r }, g{ g }, b{ b }, a{ a } {}
};

struct VertexTexture
{
	float x, y, z;
	float u, v;
	VertexTexture(float x, float y, float z, float u, float v) :
		x{ x }, y{ y }, z{ z }, u{ u }, v{ v } {}

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
	GLint matrixLocationColor;
	GLint matrixLocationTexture;
	GLint samplerLocationTexure;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	mWnd = glfwCreateWindow(WIDTH, HEIGHT, "Tytu³", nullptr, nullptr);
	glfwMakeContextCurrent(mWnd);
	mRenderer.init();
	setupGraphic();

	// Setup matrices.
	mPlayer.setPos(glm::vec3(0.0f, 0.0f, 0.0f));
	mPlayer.setSpeed(1.0f);
	mCameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	
	mProjMatrix = glm::perspective(
		glm::radians(90.0f), // Fov
		WIDTH / static_cast<float>(HEIGHT), // Aspect ratio
		1.0f, // Near clip
		1000.0f // Far clip		
	);

	startLoop();

	glfwDestroyWindow(mWnd);
}

void Application::setupGraphic()
{
	ifstream file;
	Image img;
	string vertexShader, fragmentShader;
	GLuint coloredVbo, texturedVbo;
	vector<VertexColor> bufferColor;
	vector<VertexTexture> bufferTexture;

	gl::ClearColor(0.1875f, 0.8359375f, 0.78125f, 1.0f);
	gl::Viewport(0, 0, WIDTH, HEIGHT);
	mRenderer.setDepthTest(true);
	gl::Enable(gl::BLEND);
	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

	// Fill up buffers
	// bufferColor.assign({});

	bufferTexture.assign({
		// Background
		VertexTexture{ -15.0f, -6.0f, 0.0f, 0.0f, 0.0f },
		VertexTexture{ -15.0f, 6.0f, 0.0f, 0.0f, 1.0f },
		VertexTexture{ 15.0f, 6.0f, 0.0f, 1.0f, 1.0f },
		VertexTexture{ 15.0f, 6.0f, 0.0f, 1.0f, 1.0f },
		VertexTexture{ 15.0f, -6.0f, 0.0f, 1.0f, 0.0f },
		VertexTexture{ -15.0f, -6.0f, 0.0f, 0.0f, 0.0f },

		// Paralax background
		VertexTexture{ -120.0f, -50.0f, -12.0f, 0.0f, 0.0f },
		VertexTexture{ -120.0f, 50.0f, -12.0f, 0.0f, 1.0f },
		VertexTexture{ 120.0f, 50.0f, -12.0f, 1.0f, 1.0f },
		VertexTexture{ 120.0f, 50.0f, -12.0f, 1.0f, 1.0f },
		VertexTexture{ 120.0f, -50.0f, -12.0f, 1.0f, 0.0f },
		VertexTexture{ -120.0f, -50.0f, -12.0f, 0.0f, 0.0f },

		// Ramp
		VertexTexture{ -15.0f, 0.0f, 15.0f, 0.0f, 0.0f },
		VertexTexture{ -15.0f, 0.0f, 0.0f, 0.0f, 1.0f },
		VertexTexture{ 15.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		VertexTexture{ 15.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		VertexTexture{ 15.0f, 0.0f, 15.0f, 1.0f, 0.0f },
		VertexTexture{ -15.0f, 0.0f, 15.0f, 0.0f, 0.0f },

		// PLayer
		VertexTexture{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		VertexTexture{ 0.0f, PLAYER_HEIGHT, 0.0f, 0.0f, 1.0f },
		VertexTexture{ PLAYER_WIDTH, PLAYER_HEIGHT, 0.0f, 1.0f, 1.0f },
		VertexTexture{ PLAYER_WIDTH, PLAYER_HEIGHT, 0.0f, 1.0f, 1.0f },
		VertexTexture{ PLAYER_WIDTH, 0.0f, 0.0f, 1.0f, 0.0f },
		VertexTexture{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
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

	// Create color vao
	gl::GenVertexArrays(1, &mColoredVao);
	gl::GenBuffers(1, &coloredVbo);
	gl::BindVertexArray(mColoredVao);
	gl::BindBuffer(gl::ARRAY_BUFFER, coloredVbo);
	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, sizeof(VertexColor), 0);
	gl::EnableVertexAttribArray(1);
	gl::VertexAttribPointer(1, 4, gl::FLOAT, gl::FALSE_, sizeof(VertexColor), (GLvoid*)(3 * sizeof(float)));
	gl::BindVertexArray(0);

	// Create texture vao
	gl::GenVertexArrays(1, &mTexturedVao);
	gl::GenBuffers(1, &texturedVbo);
	gl::BindVertexArray(mTexturedVao);
	gl::BindBuffer(gl::ARRAY_BUFFER, texturedVbo);
	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, sizeof(VertexTexture), 0);
	gl::EnableVertexAttribArray(1);
	gl::VertexAttribPointer(1, 2, gl::FLOAT, gl::FALSE_, sizeof(VertexTexture), (GLvoid*)(3 * sizeof(float)));
	
	// Send data
	gl::BindBuffer(gl::ARRAY_BUFFER, coloredVbo);
	gl::BufferData(gl::ARRAY_BUFFER, bufferColor.size() * sizeof(VertexColor),
		bufferColor.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, texturedVbo);
	gl::BufferData(gl::ARRAY_BUFFER, bufferTexture.size() * sizeof(VertexTexture),
		bufferTexture.data(), gl::STATIC_DRAW);

	// Load textures;
	file.open("image.png", ios_base::binary);
	PngCodec().decode(file, img);
	file.close();
	mBackgroundTex.create(img);
	file.open("image1.png", ios_base::binary);
	PngCodec().decode(file, img);
	file.close();
	mParalaxBackgroundTex.create(img);
	file.open("image2.png", ios_base::binary);
	PngCodec().decode(file, img);
	file.close();
	mRampTex.create(img);
	file.open("player.png", ios_base::binary);
	PngCodec().decode(file, img);
	file.close();
	mPlayerTex.create(img);

	// Setup sampler
	gl::GenSamplers(1, &mSampler);
	gl::SamplerParameteri(mSampler, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
	gl::SamplerParameteri(mSampler, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
	gl::SamplerParameteri(mSampler, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
	gl::SamplerParameteri(mSampler, gl::TEXTURE_MAG_FILTER, gl::LINEAR);

	// Setup gizmo
	setupGizmo();

	cout << gl::GetError() << endl;
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
		accumulator += diff.count();

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

	mPlayer.setMoveUp(glfwGetKey(mWnd, GLFW_KEY_W) == GLFW_PRESS ? true : false);
	mPlayer.setMoveDown(glfwGetKey(mWnd, GLFW_KEY_S) == GLFW_PRESS ? true : false);
	mPlayer.setMoveLeft(glfwGetKey(mWnd, GLFW_KEY_A) == GLFW_PRESS ? true : false);
	mPlayer.setMoveRight(glfwGetKey(mWnd, GLFW_KEY_D) == GLFW_PRESS ? true : false);
}

void Application::calculateLogic(double deltaTime)
{
	mViewMatrix = glm::lookAt(
		glm::vec3(0.0f, 3.0f, CAMERA_DISTANCE) + mCameraPos, // Camera position
		glm::vec3(0.0f, PLAYER_HEIGHT, 0.0f) + mCameraPos, // Look at the player head
		glm::vec3(0.0f, 1.0f, 0.0f) // Up
		);

	mPlayer.update(deltaTime);
}

void Application::renderScene()
{
	static GLuint samplerLocationTexture = mShaderTexture.getLocation("sampler1");
	static GLuint matrixLocationTexture = mShaderTexture.getLocation("mvpMatrix");
	static GLuint matrixLocationColor = mShaderColor.getLocation("mvpMatrix");
	
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Render colored
	mRenderer.bindShader(mShaderColor);

	gl::BindVertexArray(mGizmoVao);
	gl::DrawArrays(gl::LINES, 0, 6);

	// Render texutred
	mRenderer.bindShader(mShaderTexture);

	gl::UniformMatrix4fv(matrixLocationTexture, 1, gl::FALSE_, &(mProjMatrix * mViewMatrix)[0][0]);
	gl::Uniform1i(samplerLocationTexture, 0);
	gl::BindSampler(0, mSampler);
	gl::BindVertexArray(mTexturedVao);

	mRenderer.bindTexture(mParalaxBackgroundTex, 0);
	gl::DrawArrays(gl::TRIANGLES, 6, 6);

	mRenderer.bindTexture(mBackgroundTex, 0);
	gl::DrawArrays(gl::TRIANGLES, 0, 6);

	mRenderer.bindTexture(mRampTex, 0);
	gl::DrawArrays(gl::TRIANGLES, 12, 6);

	gl::UniformMatrix4fv(matrixLocationColor, 1, gl::FALSE_,
		&(mProjMatrix * mViewMatrix * glm::translate(mPlayer.getPos()))[0][0]);
	mRenderer.bindTexture(mPlayerTex, 0);
	gl::DrawArrays(gl::TRIANGLES, 18, 6);

	glfwSwapBuffers(mWnd);
}

void Application::setupGizmo()
{
	vector<VertexColor> buffer;
	GLuint vbo;

	gl::GenVertexArrays(1, &mGizmoVao);
	gl::GenBuffers(1, &vbo);
	gl::BindVertexArray(mGizmoVao);
	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, sizeof(VertexColor), 0);
	gl::EnableVertexAttribArray(1);
	gl::VertexAttribPointer(1, 4, gl::FLOAT, gl::FALSE_, sizeof(VertexColor), (GLvoid*)(3 * sizeof(float)));

	// Fill up buffer
	buffer.assign({
		VertexColor{ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		VertexColor{ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		VertexColor{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
		VertexColor{ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
		VertexColor{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		VertexColor{ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
	});

	gl::BufferData(gl::ARRAY_BUFFER, buffer.size() * sizeof(VertexColor), buffer.data(), gl::STATIC_DRAW);

	gl::BindVertexArray(0);
}

