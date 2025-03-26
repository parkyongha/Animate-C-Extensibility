#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <future>

extern "C" {
#include "jsapi.h"

	void testAsyncSocket(std::string& message);
}