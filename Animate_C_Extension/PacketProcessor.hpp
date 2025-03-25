#pragma once

#include <unordered_map>
#include <queue>
#include <string>
#include <ranges>

#include "myUtil.hpp"

class PacketProcessor {

	using dataDic = std::unordered_map<std::string, std::vector<std::string>>;

private:
	std::string buffer; // 소켓에서 받은 데이터를 임시 저장
	dataDic parsedData;

public:
	static PacketProcessor& getInstance() {
		static PacketProcessor instance;
		return instance;
	}

	// 복사 생성자와 대입 연산자 삭제
	PacketProcessor(const PacketProcessor&) = delete;
	PacketProcessor& operator=(const PacketProcessor&) = delete;

	PacketProcessor() {

	}

	std::wstring test() {
		return L"HI???";
	}

	// 소켓에서 받은 데이터를 추가
	void appendData(const std::string& data) {
		buffer.append(data);

		parse();
	}

	// 프로토콜에 맞춰 버퍼를 파싱하는 함수
	void parse() {
		size_t pos = 0;

		while ((pos = buffer.find('\n')) != std::string::npos) {
			std::string line = buffer.substr(0, pos);

			buffer.erase(0, pos + 1);

			// ":(클론)" 을 기준으로 키와 값을 분리
			std::vector<std::string_view> colonSplit = split(line, ':');

			std::string jsflName = std::string(colonSplit[0]);

			std::string_view projectNames = colonSplit[1];

			if (!projectNames.empty()) {
				auto view = split(projectNames, ',') | std::views::transform([](std::string_view sv) {
					return std::string(sv);
					});

				std::vector<std::string> projectNameSplit(view.begin(), view.end());

				parsedData.insert({ jsflName, projectNameSplit });
			}

			// 추가적인 프로토콜 검증 및 에러 처리 로직 구현 가능
		}
	}

	const dataDic& getParsedData() const {
		return parsedData;
	}
};

