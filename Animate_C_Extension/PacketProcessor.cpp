#include "PacketProcessor.h"
#include "myUtil.h"
#include <ranges>

PacketProcessor& PacketProcessor::GetInstance() {
    static PacketProcessor instance;
    return instance;
}

PacketProcessor::PacketProcessor() {}

void PacketProcessor::AppendData(const std::string& data) {
    buffer.append(data);
    parse();
}

void PacketProcessor::parse() {
    size_t pos = 0;

    std::string line = buffer.c_str();
    buffer.erase();

    // ":"를 기준으로 키와 값을 분리
    std::vector<std::string_view> colonSplit = split(line, ':');

    std::string jsflName = std::string(colonSplit[0]);
    std::string_view projectNames = colonSplit[1];

    if (!projectNames.empty()) {
        auto view = split(projectNames, ',') | std::views::transform([](std::string_view sv) {
            return std::string(sv);
            });

        if (parsedData.find(jsflName) != parsedData.end()) {
            parsedData[jsflName].insert(parsedData[jsflName].end(), view.begin(), view.end());
        }
        else {
            std::vector<std::string> projectNameSplit(view.begin(), view.end());
            parsedData.insert({ jsflName, projectNameSplit });
        }
    }
}

const std::vector<std::string>& PacketProcessor::GetParsedDataByName(const std::wstring& wsv) {
    std::string str;
    str.assign(wsv.begin(), wsv.end());

    return parsedData[str];
}

const std::vector<std::string>& PacketProcessor::GetParsedDataByName(const std::string& sv) {
    return parsedData[sv];
}
