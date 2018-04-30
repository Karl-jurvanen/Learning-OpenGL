#pragma once

#include <GL/glew.h>
//assertion for debugging __debugbreak() is visual studio compiler function
#define Assert(x) if(!(x)) __debugbreak();
//wrapper for OpenGL functions to break if an error is thrown
//first clears previous errors, then calls the function and if an error is thrown, breaks
#ifdef DEBUG
#define GLCall(x) GLClearError();\
	x;\
	Assert(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif // DEBUG



void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
