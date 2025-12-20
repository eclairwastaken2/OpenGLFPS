#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <stdexcept>

#include "shader.h"
#include "camera.h"
#include "cameraController.h"

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

class Window {
public: 
	Window(int w, int h, const char* title)
	{
		if (!glfwInit())
			throw std::runtime_error("GLFW init failed"); 

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

		window_ = glfwCreateWindow(w, h, title, nullptr, nullptr);
		if (!window_)
			throw std::runtime_error("Failed to create window"); 

		glfwMakeContextCurrent(window_); 
	}

	~Window()
	{
		glfwDestroyWindow(window_); 
		glfwTerminate(); 
	}

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	GLFWwindow* get() const { return window_;  }

private: 
	GLFWwindow* window_; 
};

class VertexArray
{
public:
	VertexArray() { glGenVertexArrays(1, &id_); }
	~VertexArray() { glDeleteVertexArrays(1, &id_); }

	//this line below is how c++ copy functions and for copy functions case:
	//	VertexArray b = a; does VertexArray b(a); pass a by reference
	//We dont want VertexArray to be able to do that so its auto deleted.
	VertexArray(const VertexArray&) = delete;
	//line below same case but for a = b -> b.operator=(a);
	VertexArray& operator=(const VertexArray&) = delete;
	void bind() const { glBindVertexArray(id_); }

private:
	unsigned int id_ = 0;
};

class Buffer {
public: 
	explicit Buffer(GLenum type) : type_(type)
	{
		glGenBuffers(1, &id_); 
	}

	~Buffer() { glDeleteBuffers(1, &id_); }

	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete; 

	void bind() const { glBindBuffer(type_, id_); }

private:
	unsigned int id_ = 0; 
	GLenum type_; 
};

class Texture2D
{
public:
	Texture2D() { glGenTextures(1, &id_); }
	~Texture2D() { glDeleteTextures(1, &id_); }

	Texture2D(const Texture2D&) = delete; 
	Texture2D& operator=(const Texture2D&) = delete; 

	void bind(GLuint unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, id_);
	}

	void setWrap(GLenum s, GLenum t) const
	{
		bind(0); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t); 
	}

	void setFilter(GLenum min, GLenum mag) const
	{
		bind(0); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag); 
	}

	void upload(int width, int height, GLenum format, const unsigned char* data) const
	{
		bind(0); 
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
			format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); 
	}

private:
	unsigned int id_ = 0; 
};

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto* controller =
		static_cast<CameraController*>(glfwGetWindowUserPointer(window));

	if (controller)
		controller->onMouseMove(xpos, ypos);
}

static void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset)
{
	auto* controller =
		static_cast<CameraController*>(glfwGetWindowUserPointer(window));

	if (controller)
		controller->onScroll(yoffset);
}

constexpr int MAP_W = 10; 
constexpr int MAP_H = 10; 

const char* levelMap = 
"##########"
"#........#"
"#..##....#"
"#........#"
"#....##..#"
"#........#"
"#..##....#"
"#........#"
"#........#"
"##########";

constexpr float PLAYER_RADIUS = 0.2f; // collision padding

glm::vec3 findSpawnPosition()
{
	for (int z = 0; z < MAP_H; z++)
	{
		for (int x = 0; x < MAP_W; x++)
		{
			if (levelMap[z * MAP_W + x] == '.')
			{
				// center of empty cell
				return glm::vec3(
					x + 0.5f,
					1.6f,
					z + 0.5f
				);
			}
		}
	}
	throw std::runtime_error("No valid spawn point!");
}

Camera camera(findSpawnPosition());

CameraController cameraController(camera, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);

bool isWall(float x, float z) 
{
	int gx = int(x);
	int gz = int(z);

	if (gx < 0 || gz < 0 || gx >= MAP_W || gz >= MAP_H)
		return true;

	return levelMap[gz * MAP_W + gx] == '#';
}

int main() try
{
	Window window(800, 600, "RAII OpenGL"); 
	glfwSetWindowUserPointer(window.get(), &cameraController);

	glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);
	glfwSetCursorPosCallback(window.get(), mouse_callback); 
	glfwSetScrollCallback(window.get(), scroll_callback); 
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

		if (isWall(camera.Position.x, camera.Position.z))
		{
			camera.Position.x = oldPos.x;

			if (isWall(camera.Position.x, camera.Position.z))
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

		for (int z = 0; z < MAP_H; z++)
		{
			for (int x = 0; x < MAP_W; x++)
			{
				if (levelMap[z * MAP_W + x] == '#')
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	glViewport(0, 0, width, height);
}
