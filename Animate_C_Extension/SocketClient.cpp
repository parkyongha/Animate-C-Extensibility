#include "SocketClient.h"
#include "PacketProcessor.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

// SOCKET에서 "\n"까지 비동기적으로 읽어, 읽은 메시지를 message에 저장하는 함수
std::future<void> async_read_message(SOCKET sock, std::string& message) {
	return std::async(std::launch::async, [&sock, &message]() {
		std::string temp;
		char ch;
		int result;
		while (true) {
			result = recv(sock, &ch, 1, 0);
			if (result <= 0) { // 에러 발생 또는 연결 종료
				break;
			}
			if (ch == '\n') { // 종료 문자 도착
				break;
			}
			temp.push_back(ch);
		}
		message = std::move(temp);
		});
}

void testAsyncSocket(std::string& message) {
	WSADATA wsaData;

	// Winsock 초기화
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		std::cerr << "WSAStartup 실패: " << iResult << std::endl;
		return;
	}

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		std::cerr << "소켓 생성 실패: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}

	// 서버 주소 설정 (127.0.0.1:12345)
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(50313);
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

	// 서버에 연결
	iResult = connect(sock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));

	if (iResult == SOCKET_ERROR) {
		std::cerr << "서버 연결 실패: " << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	std::cout << "서버에 연결됨." << std::endl;

	message = "Connect Server";

	// 비동기적으로 메시지 읽기 (읽은 메시지는 message에 저장됨)
	auto future = async_read_message(sock, message);

	// 다른 작업 수행 가능...

	// 비동기 작업 완료 대기
	future.get();
	PacketProcessor::getInstance().appendData(message);
	auto data = PacketProcessor::getInstance().getParsedDataByName("HI");

	for (auto msg : data) {
		std::cout << "받은 메시지: " << msg << '\n';
	}

	// 소켓 닫기 및 Winsock 종료
	/*closesocket(sock);
	WSACleanup();*/
}
