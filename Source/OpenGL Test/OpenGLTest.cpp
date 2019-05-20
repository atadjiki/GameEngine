#include "Source/Engine/OpenGLRender.h"
#include "Source/Engine/SmartPointer.h"
#include "Source/Engine/GameObject.h"

//what a sample game loop would look like
int main(void)
{
	//Initialize oeverything we need for OpenGL
	if (InitOGL() < 0) {
		return -1;
	}

	//create a sample render object and add it to the queue
	const RenderInfo * cube = BuildTestRenderInfo(); //describes a cube
	SmartPointer<GameObject> * gameObject = new SmartPointer<GameObject>(new GameObject());
	(*gameObject)->physics->transform.position = Math::Vector4(5, 0, 0);
	AddRenderInfo(cube, gameObject);

	float translation = 0;
	float maxTranslation = 5;
	float increment = 0.1f;
	bool forwards = true;

	do {

		if (forwards) {
			translation += increment;
		}
		else {
			translation -= increment;
		}

		if (translation >= maxTranslation) {
			forwards = false;
		}
		else if (translation <= -1 * maxTranslation) {
			forwards = true;
		}

		(*gameObject)->physics->transform.position = Math::Vector4(translation, 0, 0);

		DoRender();

	} // Check if the ESC key was pressed or the window was closed
	while (RenderCondition());

	DisposeOGL();

	return 0;
}