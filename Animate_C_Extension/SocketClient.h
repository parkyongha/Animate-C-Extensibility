#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <string>
#include <unordered_map>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

extern "C" {
#include "jsapi.h"

	JSBool tryConnectSocketClient(std::wstring& message);
}