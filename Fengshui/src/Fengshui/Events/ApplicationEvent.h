#pragma once

#include "Event.h"

namespace Fengshui
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: (" << m_Width << ", " << m_Height << ")";
			return ss.str();
		}

		static EventType GetStaticType() {
			return EventType::WindowResize;
		} virtual EventType GetEventType() const override {
			return GetStaticType();
		} virtual const char* GetName() const override {
			return "WindowResize";
		};
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		static EventType GetStaticType() {
			return EventType::WindowClose;
		} virtual EventType GetEventType() const override {
			return GetStaticType();
		} virtual const char* GetName() const override {
			return "WindowClose";
		};
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		static EventType GetStaticType() {
			return EventType::AppTick;
		} virtual EventType GetEventType() const override {
			return GetStaticType();
		} virtual const char* GetName() const override {
			return "AppTick";
		};
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		static EventType GetStaticType() {
			return EventType::AppUpdate;
		} virtual EventType GetEventType() const override {
			return GetStaticType();
		} virtual const char* GetName() const override {
			return "AppUpdate";
		};
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		static EventType GetStaticType() {
			return EventType::AppRender;
		} virtual EventType GetEventType() const override {
			return GetStaticType();
		} virtual const char* GetName() const override {
			return "AppRender";
		};
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}