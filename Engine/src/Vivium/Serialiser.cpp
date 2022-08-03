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

	Serialiser::Serialiser(const Stream::Flag& flag)
		: m_Flags(flag)
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

		switch (Stream::GetMode(m_Flags)) {
		case Stream::Flag::BINARY:
			m_Stream.out = new std::ofstream(path, std::ios::binary | std::ofstream::trunc); break;
		case Stream::Flag::TEXT:
			m_Stream.out = new std::ofstream(path, std::ios::trunc); break;
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