#pragma once

#include "Utilities.h"

// Stores version number in format major.minor.patch
struct ENGINE_API VersionNumber {
private:
    // Initialises major, minor, and patch from a version string
    void m_ConstructFromString(const char* string);

public:
    uint16_t major;
    uint16_t minor;
    uint16_t patch;

    VersionNumber();
    VersionNumber(uint16_t major, uint16_t minor, uint16_t patch);
    VersionNumber(const char* version_string);
    VersionNumber(const std::string& version_string);
    VersionNumber(const VersionNumber& other);

    void operator=(const char* version_string);
    void operator=(const std::string& version_string);
};

std::string to_string(const VersionNumber& version_number);

bool operator==(const VersionNumber& a, const VersionNumber& b);

std::ostream& operator<<(std::ostream& os, const VersionNumber& n);