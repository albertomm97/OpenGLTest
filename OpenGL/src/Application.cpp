#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

// Our vertices for drawing a square (duplicates vertices are removed)
static const float vertexBufferData[] = {
	 //     positions	// texture coords //		
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
};

// Our indices for not using duplicate vertices for drawing
unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
};

void Init();
void processInput(GLFWwindow* window);

// Our window
GLFWwindow* window;

int main()
{
	// Initializes libraries and window
	Init();

	// VertexArray
	VertexArray va;

	// VertexBuffer
	VertexBuffer vb(vertexBufferData, 4 * 5 * sizeof(float));

	// VertexBufferLayout
	VertexBufferLayout layout;
	layout.Push<float>(3); // Positions layout = 0
	std::cout << layout.GetStride() << std::endl;
	layout.Push<float>(2); // Texture	layout = 1
	std::cout << layout.GetStride() << std::endl;

	va.AddBuffer(vb, layout);
	
	// IndexBuffer
	IndexBuffer ib(indices, 6);

	// Use our basic shader
	Shader shader("res/shaders/Basic.shader"); // parse shader to string and compile
	shader.Bind();							   // bind shader (glUseProgram)
	shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

	// Textures
	Texture texture("res/textures/wall.jpg");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	// Renderer object
	Renderer renderer;
	
	int i = 0;
	float timeValue, greenValue;
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
	{
		// Process user input
		processInput(window);
		
		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 1.0, 0.0));
		shader.SetUniform4Mat("u_Transform", trans);

		// Draw
		timeValue = glfwGetTime();
		greenValue = sin(timeValue) / 2.0f + 0.5f;
		shader.SetUniform4f("u_Color", 0.0f, greenValue, 0.0f, 1.0f);
		

		renderer.Clear();
		renderer.Draw(va, ib, shader);
		
		// Check events and swap the buffers
		glfwSwapBuffers(window);
        glfwPollEvents();
    }
   
    glfwTerminate();
    return 0;
}

void Init()
{
	/* Initialize GLWF library */
	glewExperimental = true;  // Needed for core profile
	if (!glfwInit())
		fprintf(stderr, "Failed to initialize GLFW\n");

	/* Window options */
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window); // Initializes GLEW
	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		// Failed to initialize GLEW
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	// For textures transparency
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void processInput(GLFWwindow* window)
{

}