extern "C"
{
#include "jsapi.h"
}

#include "SocketClient.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "PacketProcessor.hpp"

#pragma comment(lib, "ws2_32.lib")

std::unordered_map<std::string, std::string> myMap;

void doRead(tcp::socket& socket, boost::asio::streambuf& buffer) {
	// 입력 대기 끝에 문자에 "\n"이 들어와야 함수 호출됨
	boost::asio::async_read_until(socket, buffer, "\0",
		[&socket, &buffer](const boost::system::error_code& ec, std::size_t bytes_transferred) {
			if (!ec) {
				// 버퍼에 담긴 데이터를 string으로 변환합니다.
				std::istream is(&buffer);
				std::string message(std::istreambuf_iterator<char>(is), {});

				// 받은 데이터를 map에 저장합니다.
				PacketProcessor::getInstance().appendData(message);

				std::cout << "받은 메시지: " << message << std::endl;

				// 재귀적으로 다음 메시지를 기다립니다.
				doRead(socket, buffer);
			}
			else {
				std::cerr << "읽기 오류: " << ec.message() << std::endl;
			}
		});
}

bool tryConnectSocketClient(std::wstring& message) {
	try {
		boost::asio::io_context io_context;
		tcp::resolver resolver(io_context);

		auto endpoints = resolver.resolve("127.0.0.1", "50313");

		tcp::socket socket(io_context);
		boost::asio::connect(socket, endpoints);

		boost::asio::streambuf buffer;

		// 비동기 읽기를 시작합니다. "\n" 문자가 나올 때까지 읽습니다.
		doRead(socket, buffer);

		// io_context.run()은 비동기 작업이 완료될 때까지 실행됩니다.
		io_context.run();
	}
	catch (std::exception& e) {
		std::cerr << "예외 발생: " << e.what() << std::endl;
	}
}