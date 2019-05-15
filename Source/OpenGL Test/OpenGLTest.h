#pragma once
#include <windows.h>  // For MS Windows

#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw3.h
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
using namespace glm;

#include "common/shader.hpp"

GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
GLuint VertexArrayID;
// This will identify our vertex buffer
GLuint vertexbuffer;
GLuint programID;