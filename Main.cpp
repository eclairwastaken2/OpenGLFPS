#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <stdexcept>

#include "camera/camera.h"
#include "camera/cameraController.h"
#include "core/window.h"
#include "geometry/cube.h"
#include "gl/mesh.h"
#include "gl/vertexArray.h"
#include "gl/buffer.h"
#include "gl/texture2d.h"
#include "gl/shader.h"
#include "gl/model.h"
#include "renderer/levelVisuals.h"
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
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 1600;

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
	Window window(1800, 1600, "RAII OpenGL");
	glfwSetWindowUserPointer(window.get(), &cameraController);

	registerCallbacks(window.get());
	glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to init GLAD");
	}
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	Shader shader("vertex.vs", "frag.fs");
	Shader lightingShader("light_caster.vs", "light_caster.fs");

	Mesh cubeMesh = MakeCube();

	Texture2D texture1;
	texture1.setWrap(GL_REPEAT, GL_REPEAT);
	texture1.setFilter(GL_LINEAR, GL_LINEAR);

	Texture2D woodTexture =
		Texture2D::Builder()
		.wrap(GL_REPEAT, GL_REPEAT)
		.filter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR)
		.fromFile("container.jpg");


	int w, h, channels;
	unsigned char* data = stbi_load("container.jpg", &w, &h, &channels, 0);
	texture1.upload(w, h, GL_RGB, data);
	stbi_image_free(data);
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);

	Model ourModel("resources/objects/backpack/backpack.obj");

	LevelVisuals visuals;
	visuals.cubeMesh = &cubeMesh;
	visuals.propModel = &ourModel; 
	visuals.woodTexture = &woodTexture; 
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
		//shader.use();
		lightingShader.use();
		lightingShader.setVec3("light.position", camera.Position);
		lightingShader.setVec3("light.direction", camera.Front);
		lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(20.5f)));
		lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(25.5f)));
		lightingShader.setVec3("viewPos", camera.Position);

		// light properties
		lightingShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
		// we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
		// each environment and lighting type requires some tweaking to get the best out of your environment.
		lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("light.constant", 1.0f);
		lightingShader.setFloat("light.linear", 0.09f);
		lightingShader.setFloat("light.quadratic", 0.032f);

		// material properties
		lightingShader.setFloat("material.shininess", 32.0f);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("projection", projection);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		//texture1.bind(0);
		//for (int z = 0; z < level.getH(); z++)
		//{
		//	for (int x = 0; x < level.getW(); x++)
		//	{
		//		//FLOOR
		//		glm::mat4 floorModel = glm::mat4(1.0f);
		//		floorModel = glm::translate(
		//			floorModel,
		//			glm::vec3(x + 0.5f, -6.0f, z + 0.5f)
		//		);
		//		lightingShader.setMat4("model", floorModel);
		//		cubeMesh.draw(lightingShader);

		//		//CEILING
		//		glm::mat4 ceilingModel = glm::mat4(1.0f);
		//		ceilingModel = glm::translate(
		//			ceilingModel,
		//			glm::vec3(x + 0.5f, 6.0f, z + 0.5f)
		//		);

		//		lightingShader.setMat4("model", ceilingModel);
		//		cubeMesh.draw(lightingShader);
		//	}
		//}


		//for (int z = 0; z < level.getH(); z++)
		//{
		//	for (int x = 0; x < level.getW(); x++)
		//	{
		//		if (level.at(x, z) == '#')
		//		{
		//			texture1.bind(0);
		//			glm::mat4 model = glm::mat4(1.0f);
		//			model = glm::translate(model, glm::vec3((float)x, 0.0f, (float)z));
		//			lightingShader.setMat4("model", model);
		//			cubeMesh.draw(lightingShader);
		//		}
		//		else if (level.at(x, z) == 'B')
		//		{
		//			glm::mat4 model = glm::mat4(1.0f);
		//			model = glm::translate(model, glm::vec3((float)x, 0.0f, (float)z));
		//			glm::vec3 scaleVector = glm::vec3(0.3f, 0.3f, 0.3f);
		//			model = glm::scale(model, scaleVector);
		//			lightingShader.setMat4("model", model);
		//			ourModel.Draw(lightingShader);
		//		}
		//	}
		//}

		glfwSwapBuffers(window.get());
		glfwPollEvents();
	}
	return 0;
}
catch (const std::exception& e) {
	std::cerr << e.what() << std::endl;
	return -1;
}