#pragma once

#include <unordered_map>
#include <queue>
#include <string>

class PacketProcessor {

	using dataDic = std::unordered_map<std::wstring, std::queue<std::string>>;

private:
	std::string buffer; // 소켓에서 받은 데이터를 임시 저장
	dataDic parsedData;

public:
	static PacketProcessor& getInstance() {
		static PacketProcessor instance; // C++11 이후, thread-safe 초기화 보장
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
		// 예시: 프로토콜이 "키:값\n" 형태라고 가정
		size_t pos = 0;

		while ((pos = buffer.find('\n')) != std::string::npos) {
			std::string line = buffer.substr(0, pos);

			buffer.erase(0, pos + 1);

			// ":(클론)" 을 기준으로 키와 값을 분리
			size_t sep = line.find(':');

			if (sep != std::string::npos) {
				std::wstring key = std::wstring(line.begin(), line.begin() + sep);
				std::string value = line.substr(sep + 1);
				parsedData[key].push(value);
			}

			// 추가적인 프로토콜 검증 및 에러 처리 로직 구현 가능
		}
	}

	const dataDic& getParsedData() const {
		return parsedData;
	}
};

