#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <stdexcept>

#include "shader.h"
#include "camera/camera.h"
#include "camera/cameraController.h"
#include "core/window.h"
#include "gl/vertexArray.h"
#include "gl/buffer.h"
#include "gl/texture2d.h"
#include "input/callbacks.h"
#include "world/level.h"

#define GL_CHECK(x) \
    x; \
    { \
        GLenum err = glGetError(); \
        if (err != GL_NO_ERROR) \
            std::cerr << "OpenGL error (" \
                      << glErrorToString(err) \
                      << ") at " << __FILE__ \
                      << ":" << __LINE__ << std::endl; \
    }

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f; 
float lastY = SCR_HEIGHT / 2.0f; 
bool firstMouse = true; 

float deltaTime = 0.0f; 
float lastFrame = 0.0f; 



constexpr float PLAYER_RADIUS = 0.2f; // collision padding


Level level; 

Camera camera(level.findSpawn());
CameraController cameraController(camera, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);


int main() try
{
	Window window(800, 600, "RAII OpenGL"); 
	glfwSetWindowUserPointer(window.get(), &cameraController);

	registerCallbacks(window.get()); 
	glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR); 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to init GLAD");
	}
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST); 
	Shader shader("vertex.vs", "frag.fs");

	float vertices[] =
	{
		   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	VertexArray vao;
	Buffer vbo(GL_ARRAY_BUFFER);
	Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);

	vao.bind();
	vbo.bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Texture2D texture1;
	texture1.setWrap(GL_REPEAT, GL_REPEAT);
	texture1.setFilter(GL_LINEAR, GL_LINEAR);

	int w, h, channels;
	unsigned char* data = stbi_load("container.jpg", &w, &h, &channels, 0);
	texture1.upload(w, h, GL_RGB, data);
	stbi_image_free(data);


	while (!glfwWindowShouldClose(window.get())) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		glm::vec3 oldPos = camera.Position;

		// move camera from input
		cameraController.processKeyboard(window.get(), deltaTime);

		if (level.isWall(camera.Position.x, camera.Position.z))
		{
			camera.Position.x = oldPos.x;

			if (level.isWall(camera.Position.x, camera.Position.z))
			{
				camera.Position.z = oldPos.z;
			}
		}

		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader.use();

		texture1.bind(0);
		vao.bind();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);

		// render boxes
		vao.bind();

		for (int z = 0; z < level.getH(); z++)
		{
			for (int x = 0; x < level.getW(); x++)
			{
				if (level.at(x, z) == '#')
				{
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3((float)x, 0.0f, (float)z));
					shader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
		}

		glfwSwapBuffers(window.get());
		glfwPollEvents();
	}


	return 0;
}
catch (const std::exception& e) {
	std::cerr << e.what() << std::endl;
	return -1;
}
