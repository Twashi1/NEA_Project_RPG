#include "Serialiser.h"

namespace Vivium {
	Serialiser::Serialiser(const Stream::Mode& mode)
		: m_Mode(mode)
	{
		m_Stream = Stream();
	}

	Stream::Stream() {}

	void Serialiser::BeginRead(const char* path) {
		if (!std::filesystem::exists(path)) { LogError("Couldn't find file at path: {}", path); }

		switch (m_Mode) {
		case Stream::Mode::BINARY:
			m_Stream.in = new std::ifstream(path, std::ios::binary); break;
		case Stream::Mode::TEXT:
			m_Stream.in = new std::ifstream(path); break;
		default:
			LogError("Mode is invalid! Class incorrectly initialised?");
		}

		Assert(m_Stream.in->is_open(), "File was not opened successfully");
	}

	void Serialiser::BeginWrite(const char* path) {
		if (!std::filesystem::exists(path)) { LogInfo("Creating file for write at {}", path); }

		switch (m_Mode) {
		case Stream::Mode::BINARY:
			m_Stream.out = new std::ofstream(path, std::ios::binary | std::ofstream::trunc); break;
		case Stream::Mode::TEXT:
			m_Stream.out = new std::ofstream(path, std::ios::trunc); break;
		default:
			LogError("Mode is invalid! Class incorrectly initialised?");
		}

		Assert(m_Stream.out->is_open(), "File was not opened/not created successfully");
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