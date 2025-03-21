#pragma once

#include <unordered_map>
#include <queue>
#include <string>

class PacketProcessor {

	using dataDic = std::unordered_map<std::wstring, std::queue<std::string>>;

private:
	std::string buffer; // ���Ͽ��� ���� �����͸� �ӽ� ����
	dataDic parsedData;

public:
	static PacketProcessor& getInstance() {
		static PacketProcessor instance; // C++11 ����, thread-safe �ʱ�ȭ ����
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
		// ����: ���������� "Ű:��\n" ���¶�� ����
		size_t pos = 0;

		while ((pos = buffer.find('\n')) != std::string::npos) {
			std::string line = buffer.substr(0, pos);

			buffer.erase(0, pos + 1);

			// ":(Ŭ��)" �� �������� Ű�� ���� �и�
			size_t sep = line.find(':');

			if (sep != std::string::npos) {
				std::wstring key = std::wstring(line.begin(), line.begin() + sep);
				std::string value = line.substr(sep + 1);
				parsedData[key].push(value);
			}

			// �߰����� �������� ���� �� ���� ó�� ���� ���� ����
		}
	}

	const dataDic& getParsedData() const {
		return parsedData;
	}
};

