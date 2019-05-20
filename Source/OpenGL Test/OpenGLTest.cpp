#include "OpenGLTest.h"
#include "Source/Engine/Transform.h"
#include <vector>

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

static std::vector<RenderInfo> RenderQueue = std::vector<RenderInfo>();

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

	return 0;
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

int main(void)
{
	
	if (InitOGL() < 0) {
		return -1;
	}

	AddRenderInfo(BuildTestRenderInfo());

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < RenderQueue.size(); i++) {
			ProcessRenderInfo(RenderQueue[i]);
		}
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	for (int i = 0; i < RenderQueue.size(); i++) {
		CleanRenderInfo(RenderQueue[i]);
	}

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}