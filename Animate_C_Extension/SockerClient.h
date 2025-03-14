#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>

extern "C" {
#include "jsapi.h"

	// Wide 문자열을 UTF-8 인코딩으로 변환하는 함수 선언
	std::string WideStringToUTF8(const std::wstring& wstr);

	// 서버에 연결을 시도하는 함수 선언
	bool connectToServer(SOCKET& sock, const char* serverIP, int port, std::wstring* resultMessage);

	// 서버와의 통신 루프를 구현한 함수 선언
	void communicationLoop(const char* serverIP, int port);

	// 서버에 연결되어 있는지 체크하는 함수 선언
	bool isConnectedSocket(SOCKET sock);

	// 클라이언트 시작 함수 선언
	// 이 함수는 VS Code 확장 서버와 연결하기 위해 사용됩니다.
	JSBool tryConnectSocketClient(std::wstring* message);
}