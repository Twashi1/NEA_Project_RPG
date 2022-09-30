#pragma once

#include "Core.h"
#include "Timer.h"
#include "Application.h"

namespace Vivium {
	class EventSystem;
	class EventHandler;

	class VIVIUM_API Event {
	protected:
		// Time since event invoked
		Timer m_InvokeTimer;

	public:
		// TODO: use some event ID
		std::string type;

		Event(const std::string& type);
		virtual ~Event() = default;

		friend EventSystem;
		friend EventHandler;
	};

	class VIVIUM_API EventHandler {
	private:
		void m_AddEvent(Ref(Event) event);

	protected:
		std::string m_HandlingType;
		std::queue<Ref(Event)> m_Events = {};

		virtual void m_HandleEvent(Ref(Event) event) = 0;

	public:
		void Update();

		EventHandler(const std::string& type);
		virtual ~EventHandler() = default;

		friend EventSystem;
	};

	class VIVIUM_API EventSystem {
	private:
		static std::vector<Ref(EventHandler)> m_EventHandlers;
		static void m_Update();

	public:
		static void AddEvent(Ref(Event) event);
		static void RegisterHandler(Ref(EventHandler) handler);
		static const std::vector<Ref(EventHandler)> GetHandlers();

		friend Application;
	};
}
