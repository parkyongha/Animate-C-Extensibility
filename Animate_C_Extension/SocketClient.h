#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <string>
#include <thread>
#include <mutex>

class SocketClient {
private:
	static SocketClient* instance;
	static std::mutex instanceMutex;

	SOCKET sock;
	std::thread receiveThread;
	bool connected;

	SocketClient();
	~SocketClient();

	void receiveLoop();

public:
	SocketClient(const SocketClient&) = delete;
	SocketClient& operator=(const SocketClient&) = delete;

	static SocketClient& GetInstance();

	bool Connect(int port);
	void Disconnect();

	bool SendData(const std::string& data);
};
