#include "Serialiser.h"

namespace Vivium {
	Stream::Stream() {}

	Stream::~Stream() {
		if (in != nullptr) { in->close(); }
		if (out != nullptr) { out->close(); }

		delete in;
		delete out;
	}

	int Stream::GetScopeLevel() const { return m_ScopeLevel; }
	int Stream::IncrementScope() { return ++m_ScopeLevel; }
	int Stream::DecrementScope() { return --m_ScopeLevel; }
	std::string Stream::GetTab() const
	{
		static const int SPACES_PER_TAB = 4;
		// Creates string full of spaces
		return std::string(m_ScopeLevel * SPACES_PER_TAB, ' ');
	}

	Serialiser::Serialiser(int flags)
		: m_Flags(flags)
	{
		m_Stream = Stream();
	}

	void Serialiser::BeginRead(const char* path) {
		if (!std::filesystem::exists(path)) { LogError("Couldn't find file at path: {}", path); }

		m_Stream.in = new std::ifstream(path, m_Flags);

		if (!m_Stream.in->is_open()) LogError("File was not opened successfully");
	}

	void Serialiser::BeginWrite(const char* path) {
		if (!std::filesystem::exists(path)) { LogInfo("Creating file for write at {}", path); }

		m_Stream.out = new std::ofstream(path, m_Flags);

		if(!m_Stream.out->is_open()) LogError("File was not opened/not created successfully");
	}

	void Serialiser::EndRead() {
		m_Stream.in->close();
		delete m_Stream.in;
		m_Stream.in = nullptr;
	}

	void Serialiser::EndWrite() {
		m_Stream.out->close();
		delete m_Stream.out;
		m_Stream.out = nullptr;
	}
}