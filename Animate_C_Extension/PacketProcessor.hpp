#pragma once

#include <unordered_map>
#include <queue>
#include <string>
#include <ranges>

#include "myUtil.hpp"

class PacketProcessor {

	using dataDic = std::unordered_map<std::string, std::vector<std::string>>;

private:
	std::string buffer; // ���Ͽ��� ���� �����͸� �ӽ� ����
	dataDic parsedData;

public:
	static PacketProcessor& getInstance() {
		static PacketProcessor instance;
		return instance;
	}

	// ���� �����ڿ� ���� ������ ����
	PacketProcessor(const PacketProcessor&) = delete;
	PacketProcessor& operator=(const PacketProcessor&) = delete;

	PacketProcessor() {

	}

	std::wstring test() {
		return L"HI???";
	}

	// ���Ͽ��� ���� �����͸� �߰�
	void appendData(const std::string& data) {
		buffer.append(data);

		parse();
	}

	// �������ݿ� ���� ���۸� �Ľ��ϴ� �Լ�
	void parse() {
		size_t pos = 0;

		while ((pos = buffer.find('\n')) != std::string::npos) {
			std::string line = buffer.substr(0, pos);

			buffer.erase(0, pos + 1);

			// ":(Ŭ��)" �� �������� Ű�� ���� �и�
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

			// �߰����� �������� ���� �� ���� ó�� ���� ���� ����
		}
	}

	const dataDic& getParsedData() const {
		return parsedData;
	}
};

