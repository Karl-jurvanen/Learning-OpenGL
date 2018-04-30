#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


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



static void GLClearError()
{
	//loop through the error list until there are no errors left.
	//glGetError returns an erbitrary error each time
	while (glGetError() != GL_NO_ERROR);
	
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	//get and error and asign it to error variable
	//will loop untin glGetError() returns 0 when there are no errors
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] ( " << std::hex << error << std::dec << " )" << function << 
			" " << file << ":" << line <<std::endl;
		return false;
	}
	return true;
}

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	std::string line;

	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else if (type != ShaderType::NONE)
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		//shader did not compile succesfully
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)alloca(lenght * sizeof(char));

		glGetShaderInfoLog(id, lenght, &lenght, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")  << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}


//create a shader from the strings and attach it to the program
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);


	//shaders are now linked to program, dont't need them anymore
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	

	/* Make the window's context current */
	glfwMakeContextCurrent(window);


	/* Initialize glew after window is created! */
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;
	

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] = {
	 	-0.5f,  -0.5f,
		 0.5f,  -0.5f,
		 0.5f,   0.5f,
		-0.5f,   0.5f
	};

	//index buffer to draw two triangles to form a square
	unsigned int indices[]{
		0,1,2,
		2,3,0
	};
	//generate data for openGL from our vertices
	unsigned int buffer;
	GLCall( glGenBuffers(1, &buffer));

	//bind selects the buffer we want to work with
	GLCall( glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall( glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));




	//enable the vertex attribute we want
	//0 here for position
	GLCall( glEnableVertexAttribArray(0));
	/*
	glVertexAttribPointer

	index = 0 (position)
	size = how many floats represent this vertex attribute
	type = float
	Normalized = should the data be normalized , here: no
	stride = amount of bytes between each vertex
	pointer = how many bytes from start of vertex to get to an attribute
			here 0 because we're setting position
	*/
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));


	//generate indexbuffer
	unsigned int ibo; //indexBufferObject
	GLCall(glGenBuffers(1, &ibo));

	
	GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall( glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		
		GLCall( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	GLCall(glDeleteProgram(shader));

	glfwTerminate();
	return 0;
}