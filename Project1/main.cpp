#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "headers/stb_image.h"

#include "headers/shader.h"

#include "headers/1dPerlinNoise.hpp"

#include <filesystem>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
	
int main()
{
	//std::cout << std::filesystem::current_path() << '\n';

	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		});

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD\n";
		return -1;
	}

	//shader
	Shader ourShader("shaders\\vertexshader.v", "shaders\\fragmentshader.f");

	float vertices[] = {
		// positions         // texture coords
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	GLuint  VAO, VBO, EBO;
	glCreateVertexArrays(1, &VAO);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);
	glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glNamedBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(VAO, 0);
	glEnableVertexArrayAttrib(VAO, 1);

	glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 0);

	glVertexArrayAttribBinding(VAO, 0, 0);
	glVertexArrayAttribBinding(VAO, 1, 1);

	//for vertex
	glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(float));

	//for texture
	glVertexArrayVertexBuffer(VAO, 1, VBO, 3 * sizeof(float), 5 * sizeof(float));

	glVertexArrayElementBuffer(VAO, EBO);


	//texture
	unsigned int texture1, texture2;

	glCreateTextures(GL_TEXTURE_2D, 1, &texture1);

	// set the texture wrapping parameters
	glTextureParameteri(texture1, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(texture1, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(texture1, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(texture1, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("Resources\\container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// Calculate the total number of mipmap levels needed
		GLsizei levels = static_cast<GLsizei>(std::floor(std::log2(std::max(width, height)))) + 1;

		// Allocate immutable texture storage (using sized internal format GL_RGB8)
		glTextureStorage2D(texture1, levels, GL_RGB8, width, height);

		// Upload the image data to base mip level (0)
		glTextureSubImage2D(texture1, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

		// Generate mipmaps directly on the texture object
		glGenerateTextureMipmap(texture1);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//texture2
	glCreateTextures(GL_TEXTURE_2D, 1, &texture2);

	glTextureParameteri(texture2, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(texture2, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(texture2, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(texture2, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("Resources\\awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// Calculate the total number of mipmap levels needed
		GLsizei levels = static_cast<GLsizei>(std::floor(std::log2(std::max(width, height)))) + 1;

		// Allocate immutable texture storage (using sized internal format GL_RGBA8)
		glTextureStorage2D(texture2, levels, GL_RGBA8, width, height);

		// Upload the image data to base mip level (0)
		glTextureSubImage2D(texture2, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// Generate mipmaps directly on the texture object
		glGenerateTextureMipmap(texture2);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//set uniform
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.77f, 0.32f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTextureUnit(0, texture1);
		glBindTextureUnit(1, texture2);

		// activate shader
		ourShader.use();

		// create transformations
		glm::mat4 model = glm::mat4(1.0f); 
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		// retrieve the matrix uniform locations
		//unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		//unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");

		ourShader.setMat4("model", model);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
