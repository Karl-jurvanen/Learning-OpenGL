#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

	float positions[6] = {
	 	-0.5f,  -0.5f,
		 0.0f,   0.5f,
		 0.5f,  -0.5f
	};

	//generate data for openGL from our vertices
	unsigned int buffer;
	glGenBuffers(1, &buffer);

	//bind selects the buffer we want to work with
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);




	//enable the vertex attribute we want
	//0 here for position
	glEnableVertexAttribArray(0);
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);

		glDrawArrays(GL_TRIANGLES, 0, 3); // start from zero, three indices

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}