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

#pragma comment(lib, "ws2_32.lib")


std::unordered_map<std::string, std::string> myMap;

void do_read(tcp::socket& socket, boost::asio::streambuf& buffer, const std::string& key) {
	// 입력 대기 끝에 문자에 "\n"이 들어와야 함수 호출됨
	boost::asio::async_read_until(socket, buffer, "\0",
		[&socket, &buffer, key](const boost::system::error_code& ec, std::size_t bytes_transferred) {
			if (!ec) {
				// 버퍼에 담긴 데이터를 string으로 변환합니다.
				std::istream is(&buffer);
				std::string message(std::istreambuf_iterator<char>(is), {});

				// 받은 데이터를 map에 저장합니다.
				myMap[key] = message;
				std::cout << "받은 메시지: " << message << std::endl;

				// 재귀적으로 다음 메시지를 기다립니다.
				do_read(socket, buffer, key);
			}
			else {
				std::cerr << "읽기 오류: " << ec.message() << std::endl;
			}
		});
}

JSBool tryConnectSocketClient(std::wstring& message) {
	try {
		boost::asio::io_context io_context;
		tcp::resolver resolver(io_context);

		auto endpoints = resolver.resolve("127.0.0.1", "50313");

		tcp::socket socket(io_context);
		boost::asio::connect(socket, endpoints);

		boost::asio::streambuf buffer;

		// 키 변수는 나중에 원하는 값으로 설정할 수 있습니다.
		std::string key = "myKey";

		// 비동기 읽기를 시작합니다. "\n" 문자가 나올 때까지 읽습니다.
		do_read(socket, buffer, key);

		// io_context.run()은 비동기 작업이 완료될 때까지 실행됩니다.
		io_context.run();
	}
	catch (std::exception& e) {
		std::cerr << "예외 발생: " << e.what() << std::endl;
	}
}