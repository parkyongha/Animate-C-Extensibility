#include "myUtil.hpp"

std::vector<std::string_view> split(std::string_view str, char delim) {
    std::vector<std::string_view> tokens;

    size_t start = 0;

    while (true) {
        size_t pos = str.find(delim, start);
        tokens.push_back(str.substr(start, pos - start));
        if (pos == std::string_view::npos)
            break;
        start = pos + 1;
    }

    return tokens;
}

unsigned short* wstr2ushort(std::wstring& ws) {
    return reinterpret_cast<unsigned short*>(&ws[0]);
}

std::string WStringToUTF8(const std::wstring& wstr)
{
    if (wstr.empty()) return {};

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, NULL, NULL);
    return str;
}