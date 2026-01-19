#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <stdexcept>

#include "animation/animator.h"
#include "animation/model_animation.h"
#include "camera/camera.h"
#include "camera/cameraController.h"
#include "core/window.h"
#include "geometry/cube.h"
#include "gl/mesh.h"
#include "gl/vertexArray.h"
#include "gl/buffer.h"
#include "gl/texture2d.h"
#include "gl/shader.h"
#include "renderer/levelVisuals.h"
#include "renderer/levelRenderer.h"
#include "input/callbacks.h"
#include "world/level.h"
#include "world/player.h"

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

// settings
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 1600;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
constexpr float PLAYER_RADIUS = 0.2f; // collision padding

void static setupStaticLighting(Shader& shader)
{
	shader.use();

	shader.setVec3("flashlight.ambient", 0.1f, 0.1f, 0.1f);
	shader.setVec3("flashlight.diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("flashlight.specular", 1.0f, 1.0f, 1.0f);

	shader.setFloat("flashlight.constant", 1.0f);
	shader.setFloat("flashlight.linear", 0.09f);
	shader.setFloat("flashlight.quadratic", 0.032f);

	shader.setFloat("material.shininess", 32.0f);
}

void static updateLightingPerFrame(Shader& shader, const Camera& camera)
{
	shader.use();

	shader.setVec3("flashlight.position", camera.Position);
	shader.setVec3("flashlight.direction", camera.Front);
	shader.setFloat("flashlight.cutOff", glm::cos(glm::radians(20.5f)));
	shader.setFloat("flashlight.outerCutOff", glm::cos(glm::radians(25.5f)));

	shader.setVec3("viewPos", camera.Position);

	glm::mat4 projection = glm::perspective(
		glm::radians(camera.Zoom),
		(float)SCR_WIDTH / (float)SCR_HEIGHT,
		0.1f,
		100.0f
	);

	shader.setMat4("projection", projection);
	shader.setMat4("view", camera.GetViewMatrix());
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Player player;
Level level("assets/levels/level.txt");
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

	// init imgui
	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplGlfw_InitForOpenGL(window.get(), GL_TRUE);
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.WantSaveIniSettings = false;

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	unsigned int hdrFBO;
	glGenFramebuffers(1, &hdrFBO);
	// create floating point color buffer
	unsigned int colorBuffer;
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// create depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	// attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Shader shader("vertex.vs", "frag.fs");
	Shader lightingShader("light_caster.vs", "light_caster.fs");
	setupStaticLighting(lightingShader);

		player.setPosition(level.findSpawn());


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

	Model ourModel("resources/objects/gems/source/gems.obj");

	LevelVisuals visuals;
	visuals.cubeMesh = &cubeMesh;
	visuals.propModel = &ourModel; 
	visuals.woodTexture = &woodTexture; 

	LevelRenderer renderer(visuals); 

	//animation models
	Shader animationShader("animation/anim_model.vs", "animation/anim_model.fs");
	Model animationModel("resources/objects/vampire/dancing_vampire.dae");
	Animation danceAnimation("resources/objects/vampire/dancing_vampire.dae", &animationModel);
	Animator animator(&danceAnimation);
	while (!glfwWindowShouldClose(window.get())) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		animator.UpdateAnimation(deltaTime);


		glfwPollEvents();
		// input
		player.setPosition(camera.Position);

		glm::vec3 oldPos = camera.Position; 

		// move camera from input
		cameraController.processKeyboard(window.get(), deltaTime);

		constexpr float CAMERA_RADIUS = 0.3f; 
		glm::vec3 nextPos = camera.Position;

		if (level.collides(nextPos.x, oldPos.z, CAMERA_RADIUS)) {
			nextPos.x = oldPos.x; 
		}

		if (level.collides(nextPos.x, nextPos.z, CAMERA_RADIUS)) {
			nextPos.z = oldPos.z; 
		}
		player.setPosition(nextPos); 
		camera.Position = nextPos;
		player.tryCollect(level);


		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//animation
		animationShader.use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		animationShader.setMat4("projection", projection);
		animationShader.setMat4("view", view);

		auto transforms = animator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			animationShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, camera.Position + camera.Front * 3.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));

		model = glm::scale(model, glm::vec3(.5f, .5f, .5f));
		animationShader.setMat4("model", model);
		animationModel.Draw(animationShader);

		//lighting

		updateLightingPerFrame(lightingShader, camera);

		renderer.render(level, lightingShader); 

		glBindFramebuffer(GL_READ_FRAMEBUFFER, hdrFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		glBlitFramebuffer(
			0, 0, SCR_WIDTH, SCR_HEIGHT,
			0, 0, SCR_WIDTH, SCR_HEIGHT,
			GL_COLOR_BUFFER_BIT,
			GL_NEAREST
		);



		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Debug");
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Render();

		glDisable(GL_DEPTH_TEST);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window.get());
	}
	return 0;
}
catch (const std::exception& e) {
	std::cerr << e.what() << std::endl;
	return -1;
}