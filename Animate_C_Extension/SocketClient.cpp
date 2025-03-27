#include "SocketClient.h"
#include "PacketProcessor.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

SOCKET sock;

void read_loop() {
	std::thread([]() {
		while (true) {
			std::string temp;
			char ch;
			int result;

			while (true) {
				result = recv(sock, &ch, 1, 0);
				if (result <= 0 || ch == '\n') {
					break;
				}
				temp.push_back(ch);
			}

			std::cout << temp;

			PacketProcessor::getInstance().appendData(temp);

			// 입력 테스트 코드
			//auto data = PacketProcessor::getInstance().getParsedDataByName("HI");

			//for (auto msg : data) {
			//	std::cout << "받은 메시지: " << msg << '\n';
			//}
		}
		}).detach();
}

bool sendMessageToServer(const std::string_view msg)
{
	int iResult = send(sock, msg.data(), msg.length(), 0);

	if (iResult == SOCKET_ERROR) {
		std::wcout << (L"send failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return false;
	}

	return true;
}

void testAsyncSocket(std::string& message) {
	WSADATA wsaData;

	// Winsock 초기화
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		std::cerr << "WSAStartup 실패: " << iResult << '\n';
		return;
	}

	// 소켓 생성
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		std::cerr << "소켓 생성 실패: " << WSAGetLastError() << '\n';
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
		std::cerr << "서버 연결 실패: " << WSAGetLastError() << '\n';
		closesocket(sock);
		WSACleanup();
		return;
	}

	std::cout << "Connect Server" << '\n';

	message = "Connect Server";

	read_loop();

	// 소켓 닫기 및 Winsock 종료
	/*closesocket(sock);
	WSACleanup();*/
}
