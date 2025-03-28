#pragma once

#include <string>
#include <vector>
#include <Windows.h>

std::vector<std::string_view> split(std::string_view str, char delim);

unsigned short* wstr2ushort(std::wstring& ws);

std::string WStringToUTF8(const std::wstring& wstr);