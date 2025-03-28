#pragma once

#include <unordered_map>
#include <queue>
#include <string>
#include <vector>

class PacketProcessor {
    using dataDic = std::unordered_map<std::string, std::vector<std::string>>;

private:
    std::string buffer;
    dataDic parsedData;

    void parse();

public:
    static PacketProcessor& GetInstance();

    PacketProcessor(const PacketProcessor&) = delete;
    PacketProcessor& operator=(const PacketProcessor&) = delete;

    PacketProcessor();

    void AppendData(const std::string& data);

    const std::vector<std::string>& GetParsedDataByName(const std::wstring& wsv);
    const std::vector<std::string>& GetParsedDataByName(const std::string& sv);
};
