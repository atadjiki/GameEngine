#include <assert.h>		// for assert()
#include <stdarg.h>		// for va_<xxx>
#include <stdio.h>		// for vsprintf_s()
#include <Windows.h>	// for OutputDebugStringA(). Uggh.. this pulls in a lot of Windows specific stuff

namespace Engine
{
	/*
	* Modified method to accomodate multiple arguments. 
	*/
	void ConsolePrint(int count, const char * i_fmt, ...)
	{
		assert(i_fmt);

		const size_t lenTemp = 256;
		char strTemp[lenTemp] = "LOG: ";

		const size_t lenOutput = lenTemp + 32;

		char strOutput[lenOutput];

		// define a variable argument list variable 
		va_list args;

		// initialize it. second parameter is variable immediately
		// preceeding variable arguments
		va_start(args, i_fmt);

		strcat_s(strTemp, i_fmt);

		for (int i = 0; i < count - 1; i++) {
			
			strcat_s(strTemp, va_arg(args, const char *));
		}

		strcat_s(strTemp, "\n");

		// (safely) print our formatted string to a char buffer
		vsprintf_s(strOutput, lenOutput, strTemp, args);

		va_end(args);

		OutputDebugStringA(strOutput);
	}

} // namespace Engine


