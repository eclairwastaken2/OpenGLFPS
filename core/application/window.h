#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <functional>

namespace Core {

	struct WindowSpecification
	{
		std::string Title;
		int32_t Width = 1800;
		int32_t Height = 1600;
		bool IsResizeable = true;
		bool VSync = true;
	};

	class Window
	{
	public:
		Window(const WindowSpecification& specification = WindowSpecification());
		~Window();

		void Create();
		void Destroy();

		void Update();

		glm::vec2 GetFramebufferSize() const;

		bool ShouldClose() const;

		GLFWwindow* GetHandle() const { return m_Handle; }
	private:
		WindowSpecification m_Specification;

		GLFWwindow* m_Handle = nullptr;

	};

}