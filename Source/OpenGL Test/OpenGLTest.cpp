#include "Source/Engine/OpenGLRender.h"

//what a sample game loop would look like
int main(void)
{
	//Initialize oeverything we need for OpenGL
	if (InitOGL() < 0) {
		return -1;
	}

	//create a sample render object and add it to the queue
	AddRenderInfo(BuildTestRenderInfo());

	do {

		DoRender();

	} // Check if the ESC key was pressed or the window was closed
	while (RenderCondition());

	DisposeOGL();

	return 0;
}