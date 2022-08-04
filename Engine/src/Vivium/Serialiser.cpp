#include "Serialiser.h"

namespace Vivium {
	Stream::Flag Stream::GetMode(const Stream::Flag& flag) {
		// TODO: this is pretty ugly
		bool isBinary = (uint8_t)flag & (uint8_t)Stream::Flag::BINARY;
		bool isText = (uint8_t)flag & (uint8_t)Stream::Flag::TEXT;

		if (isBinary)		return Stream::Flag::BINARY;
		else if (isText)	return Stream::Flag::TEXT;
		else				return Stream::Flag::INVALID;
	}

	bool Stream::GetTrunc(const Flag& flag)
	{
		return (uint8_t)flag & (uint8_t)Stream::Flag::TRUNC;
	}

	int operator|(const Stream::Flag& left, const Stream::Flag& right) { return (uint8_t)left | (uint8_t)right; }
	int operator|(const int& left, const Stream::Flag& right) { return left | (uint8_t)right; }
	int operator|(const Stream::Flag& left, const int& right) { return (uint8_t)left | right; }

	int Stream::GetTabCount() const { return m_CurrentTabCount; }
	int Stream::IncrementTabCount() { return ++m_CurrentTabCount; }
	int Stream::DecrementTabCount() { return --m_CurrentTabCount; }
	std::string Stream::GetTab() const
	{
		static const int SPACES_PER_TAB = 4;
		// Creates string full of spaces
		return std::string(m_CurrentTabCount * SPACES_PER_TAB, ' ');
	}

	Serialiser::Serialiser(const Stream::Flag& flag)
		: m_Flags(flag)
	{
		m_Stream = Stream();
	}

	Serialiser::Serialiser(const int& flags)
		: m_Flags((Stream::Flag)flags)
	{
		m_Stream = Stream();
	}


	Stream::Stream() {}

	void Serialiser::BeginRead(const char* path) {
		if (!std::filesystem::exists(path)) { LogError("Couldn't find file at path: {}", path); }

		switch (Stream::GetMode(m_Flags)) {
		case Stream::Flag::BINARY:
			m_Stream.in = new std::ifstream(path, std::ios::binary); break;
		case Stream::Flag::TEXT:
			m_Stream.in = new std::ifstream(path); break;
		default:
			LogError("Mode is invalid! Class incorrectly initialised?");
		}

		if (!m_Stream.in->is_open()) LogError("File was not opened successfully");
	}

	void Serialiser::BeginWrite(const char* path) {
		if (!std::filesystem::exists(path)) { LogInfo("Creating file for write at {}", path); }

		bool truncate = Stream::GetTrunc(m_Flags);

		// TODO: so bad

		switch (Stream::GetMode(m_Flags)) {
		case Stream::Flag::BINARY:
			if (truncate) {
				m_Stream.out = new std::ofstream(path, std::ios::binary | std::ios::trunc);
			}
			else {
				m_Stream.out = new std::ofstream(path, std::ios::binary | std::ios::app);
			} break;
		case Stream::Flag::TEXT:
			if (truncate) {
				m_Stream.out = new std::ofstream(path, std::ios::trunc);
			}
			else {
				m_Stream.out = new std::ofstream(path, std::ios::app);
			} break;
		default:
			LogError("Mode is invalid! Class incorrectly initialised?");
		}

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
	
	Stream::~Stream() {
		if (in != nullptr) { in->close(); }
		if (out != nullptr) { out->close(); }

		delete in;
		delete out;
	}
}