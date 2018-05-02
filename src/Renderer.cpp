#include "Renderer.h"

#include <iostream>

void GLClearError()
{
	//loop through the error list until there are no errors left.
	//glGetError returns an erbitrary error each time
	while (glGetError() != GL_NO_ERROR);

}

bool GLLogCall(const char* function, const char* file, int line)
{
	//get and error and asign it to error variable
	//will loop untin glGetError() returns 0 when there are no errors
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] ( " << std::hex << error << std::dec << " )" << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}