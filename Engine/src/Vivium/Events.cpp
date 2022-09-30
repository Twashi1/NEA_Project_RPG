#include "Events.h"

namespace Vivium {
	std::vector<Ref(EventHandler)> EventSystem::m_EventHandlers;

	void EventHandler::m_AddEvent(Ref(Event) event)
	{
		event->m_InvokeTimer.Start();
		m_Events.push(event);
	}

	void EventHandler::Update()
	{
		while (!m_Events.empty()) {
			// Handle first event
			m_HandleEvent(m_Events.front());
			// Pop event that's been handled
			m_Events.pop();
		}
	}
	
	EventHandler::EventHandler(const std::string& type) : m_HandlingType(type), m_Events() {}
	 
	Event::Event(const std::string& type) : type(type) {}

	void EventSystem::m_Update()
	{
		if (!m_EventHandlers.empty()) {
			for (auto event_handler : m_EventHandlers) {
				event_handler->Update();
			}
		}
	}

	void EventSystem::AddEvent(Ref(Event) event)
	{
		if (!m_EventHandlers.empty()) {
			for (auto& handler : m_EventHandlers) {
				if (handler->m_HandlingType == event->type) {
					handler->m_AddEvent(event);
				}
			}
		}
	}

	void EventSystem::RegisterHandler(Ref(EventHandler) handler)
	{
		m_EventHandlers.push_back(handler);
	}

	const std::vector<Ref(EventHandler)> EventSystem::GetHandlers()
	{
		return m_EventHandlers;
	}
}
