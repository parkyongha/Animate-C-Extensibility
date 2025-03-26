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
