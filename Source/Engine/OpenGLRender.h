#pragma once
#include <windows.h>  // For MS Windows
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW. Always include it before gl.h and glfw3.h
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/controls.hpp"

#include <Source/Engine/Vector2.h>
#include <Source/Engine/Vector3.h>
#include <Source/Engine/Matrix4.h>
#include "Source/Engine/Transform.h"

using namespace glm;

GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
GLuint VertexArrayID;

GLuint vertexbuffer; // This will identify our vertex buffer
GLuint shaderID;

static const GLfloat cube_data[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

static const GLfloat cube_color[] = {
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
};

static const GLfloat cube_uv[] = {
	0.000059f, 0.000004f,
	0.000103f, 0.336048f,
	0.335973f, 0.335903f,
	1.000023f, 0.000013f,
	0.667979f, 0.335851f,
	0.999958f, 0.336064f,
	0.667979f, 0.335851f,
	0.336024f, 0.671877f,
	0.667969f, 0.671889f,
	1.000023f, 0.000013f,
	0.668104f, 0.000013f,
	0.667979f, 0.335851f,
	0.000059f, 0.000004f,
	0.335973f, 0.335903f,
	0.336098f, 0.000071f,
	0.667979f, 0.335851f,
	0.335973f, 0.335903f,
	0.336024f, 0.671877f,
	1.000004f, 0.671847f,
	0.999958f, 0.336064f,
	0.667979f, 0.335851f,
	0.668104f, 0.000013f,
	0.335973f, 0.335903f,
	0.667979f, 0.335851f,
	0.335973f, 0.335903f,
	0.668104f, 0.000013f,
	0.336098f, 0.000071f,
	0.000103f, 0.336048f,
	0.000004f, 0.671870f,
	0.336024f, 0.671877f,
	0.000103f, 0.336048f,
	0.336024f, 0.671877f,
	0.335973f, 0.335903f,
	0.667969f, 0.671889f,
	1.000004f, 0.671847f,
	0.667979f, 0.335851f
};

struct RenderInfo {

	GLuint VertexArrayID;
	GLuint shaderID;
	GLuint MatrixID;
	GLuint Texture;
	GLuint TextureID;
	GLuint vertexbuffer;
	const GLfloat * vertex_data;
	size_t vertex_size;
	GLuint uvbuffer;
	const GLfloat * uv_data;
	size_t uv_size;
	Math::Transform transform;
};

static std::vector<RenderInfo> RenderQueue; //houses render info for our game objects

int InitOGL() {

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

																   // Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Tutorial 03 - Matrices", NULL, NULL);
	if (window == NULL) {
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	RenderQueue = std::vector<RenderInfo>();

	return 0;
}

bool DisposeOGL() {

	// Cleanup VBO and shader
	for (int i = 0; i < RenderQueue.size(); i++) {
		CleanRenderInfo(RenderQueue[i]);
	}

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	RenderQueue.~vector();
}

void ProcessRenderInfo(RenderInfo renderInfo) {

	// Use our shader
	glUseProgram(renderInfo.shaderID);

	// Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs();
	Math::Matrix4 ProjectionMatrix = Math::GLMtoMatrix4(getProjectionMatrix());
	Math::Matrix4 ViewMatrix = Math::GLMtoMatrix4(getViewMatrix());
	Math::Matrix4 ModelMatrix = Math::TranslationMatrix_Row(renderInfo.transform.position);
	Math::Matrix4 MVP = ModelMatrix * ViewMatrix * ProjectionMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(renderInfo.MatrixID, 1, GL_FALSE, &MVP.matrix[0][0]);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderInfo.Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(renderInfo.TextureID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, renderInfo.vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, renderInfo.uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void CleanRenderInfo(RenderInfo renderInfo) {

	glDeleteBuffers(1, &renderInfo.vertexbuffer);
	glDeleteProgram(renderInfo.shaderID);
	glDeleteVertexArrays(1, &renderInfo.VertexArrayID);
}

RenderInfo BuildTestRenderInfo() {
	RenderInfo renderInfo = RenderInfo();

	glGenVertexArrays(1, &renderInfo.VertexArrayID);
	glBindVertexArray(renderInfo.VertexArrayID);

	// Create and compile our GLSL program from the shaders
	renderInfo.shaderID = LoadShaders("shaders/vs.vertexshader", "shaders/fs.fragmentshader");

	// Get a handle for our "MVP" uniform
	renderInfo.MatrixID = glGetUniformLocation(renderInfo.shaderID, "MVP");

	// Load the texture
	renderInfo.Texture = loadDDS("uvtemplate.DDS");

	// Get a handle for our "myTextureSampler" uniform
	renderInfo.TextureID = glGetUniformLocation(renderInfo.shaderID, "sampler");

	renderInfo.vertex_data = cube_data;
	renderInfo.vertex_size = sizeof(cube_data);
	renderInfo.uv_data = cube_uv;
	renderInfo.uv_size = sizeof(cube_uv);

	glGenBuffers(1, &renderInfo.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderInfo.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, renderInfo.vertex_size, renderInfo.vertex_data, GL_STATIC_DRAW);

	glGenBuffers(1, &renderInfo.uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderInfo.uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, renderInfo.uv_size, renderInfo.uv_data, GL_STATIC_DRAW);

	renderInfo.transform = Math::Transform();
	renderInfo.transform.position = Math::Vector4(1, 0, 0);

	return renderInfo;
}

void AddRenderInfo(RenderInfo renderInfo) {

	RenderQueue.push_back(renderInfo);
}

void RemoveRenderInfo(RenderInfo renderInfo) {

	for (int i = 0; i < RenderQueue.size(); i++) {

		if (&renderInfo == &RenderQueue[i]) {

			RenderQueue.erase(RenderQueue.begin() + i);
		}
	}
}

void ClearRenderQueue() {

	RenderQueue.clear();
}

bool RenderCondition() {
	return glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0;
}

void DoRender() {

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < RenderQueue.size(); i++) {
		ProcessRenderInfo(RenderQueue[i]);
	}

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}
