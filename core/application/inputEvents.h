#pragma once

#include "Event.h"

#include <format>

namespace Core {

	//
	// Key Events
	//

	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return keyCode_; }
	protected:
		KeyEvent(int keycode)
			: keyCode_(keycode) {
		}

		int keyCode_;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, bool isRepeat)
			: KeyEvent(keycode), isRepeat_(isRepeat) {
		}

		inline bool IsRepeat() const { return isRepeat_; }

		std::string ToString() const override
		{
			return std::format("KeyPressedEvent: {} (repeat={})", keyCode_, isRepeat_);
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool isRepeat_;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {
		}

		std::string ToString() const override
		{
			return std::format("KeyReleasedEvent: {}", keyCode_);
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	//
	// Mouse Events
	//

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(double x, double y)
			: mouseX_(x), mouseY_(y) {
		}

		inline double GetX() const { return mouseX_; }
		inline double GetY() const { return mouseY_; }

		std::string ToString() const override
		{
			return std::format("MouseMovedEvent: {}, {}", mouseX_, mouseY_);
		}

		EVENT_CLASS_TYPE(MouseMoved)
	private:
		double mouseX_, mouseY_;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(double xOffset, double yOffset)
			: xOffset_(xOffset), yOffset_(yOffset) {
		}

		inline double GetXOffset() const { return xOffset_; }
		inline double GetYOffset() const { return yOffset_; }

		std::string ToString() const override
		{
			return std::format("MouseScrolledEvent: {}, {}", xOffset_, yOffset_);
		}

		EVENT_CLASS_TYPE(MouseScrolled)
	private:
		double xOffset_, yOffset_;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return button_; }
	protected:
		MouseButtonEvent(int button)
			: button_(button) {
		}

		int button_;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {
		}

		std::string ToString() const override
		{
			return std::format("MouseButtonPressedEvent: {}", button_);
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {
		}

		std::string ToString() const override
		{
			return std::format("MouseButtonReleasedEvent: {}", button_);
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};


}