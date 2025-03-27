#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <future>

extern "C" {
#include "jsapi.h"

	bool sendMessageToServer(const std::string_view msg);

	void testAsyncSocket(std::string& message);
}