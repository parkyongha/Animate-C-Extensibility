
//this one is for sample.cpp
extern "C"
{
#include "jsapi.h"
}

#include "SockerClient.h"
#include <string>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

SOCKET sock;

// Wide 문자열을 UTF-8 인코딩으로 변환하는 함수
std::string WideStringToUTF8(const std::wstring& wstr) {
	if (wstr.empty())
		return std::string();

	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);

	std::string result(sizeNeeded, 0);

	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &result[0], sizeNeeded, NULL, NULL);

	return result;
}

bool connectToServer(SOCKET& sock, const char* serverIP, int port, std::wstring* message) {
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	// IP 주소 변환 실패 처리
	if (InetPtonA(AF_INET, serverIP, &serverAddr.sin_addr) != 1) {
		std::wstringstream ws;
		// char*를 std::wstring으로 변환 (서버IP는 ASCII로 가정)
		ws << L"잘못된 IP 주소입니다: " << std::wstring(serverIP, serverIP + std::strlen(serverIP)) << L"\n";
		*message = ws.str();
		return false;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET) {
		std::wstringstream ws;
		ws << L"소켓 생성 실패, 오류 코드: " << WSAGetLastError() << L"\n";
		*message = ws.str();
		return false;
	}

	if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::wstringstream ws;
		ws << L"서버 연결 실패, 오류 코드: " << WSAGetLastError() << L"\n";
		*message = ws.str();
		closesocket(sock);
		return false;
	}

	std::wstringstream ws;
	ws << L"서버에 성공적으로 연결되었습니다.";
	*message = ws.str();

	return true;
}

// 서버와의 통신 루프를 구현한 함수
void communicationLoop(const char* serverIP, int port) {
	WSADATA wsaData;
	// Winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup 실패\n");
		return;
	}

	SOCKET sock;

	// 서버에 메시지 전송
	// Wide 문자열로 메시지를 정의 (UTF-16
	std::wstring wideMsg = L"call:someFunction:HelloFromAnimate 안녕";
	// UTF-8로 변환
	std::string msg = WideStringToUTF8(wideMsg);

	int bytesSent = send(sock, msg.c_str(), (int)msg.length(), 0);
	if (bytesSent == SOCKET_ERROR) {
		printf("메시지 전송 실패, 오류 코드: %d\n", WSAGetLastError());
	}
	else {
		printf("메시지 전송 성공: %s\n", msg.c_str());
	}

	// 서버로부터 응답 수신
	char buffer[512];
	int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);

	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';  // 문자열 종결

		// (1) UTF-8 → WideChar 변환
		wchar_t wbuffer[512];
		int wlen = MultiByteToWideChar(CP_UTF8, 0, buffer, -1, wbuffer, 512);
		if (wlen > 0) {
			wbuffer[wlen] = L'\0';
			// (2) WideChar 문자열을 wprintf로 출력
			wprintf(L"서버 응답: %s\n", wbuffer);
		}
		else {
			wprintf(L"MultiByteToWideChar 변환 실패\n");
		}
	}
	else {
		wprintf(L"recv 실패, 오류 코드: %d\n", WSAGetLastError());
	}

	// 소켓 종료 및 Winsock 정리
	closesocket(sock);
	WSACleanup();
}

bool isConnectedSocket(SOCKET sock) {
	// 0바이트 전송을 시도합니다.
	int ret = send(sock, "", 0, 0);

	if (ret == SOCKET_ERROR) {
		int err = WSAGetLastError();

		return false;
	}

	return true;
}
JSBool tryConnectSocketClient(std::wstring* message) {
	// 전역 변수로 선언된 소켓(sock)이 이미 연결되어 있는지 확인
	if (isConnectedSocket(sock)) {
		*message = L"이미 서버에 연결되어 있습니다.";
		return JS_TRUE;
	}

	WSADATA wsaData;
	int startupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (startupResult != 0) {
		std::wstringstream ws;
		ws << L"WSAStartup 실패, 오류 코드: " << startupResult;
		*message = ws.str();
		return JS_FALSE;
	}

	// 서버 정보 설정 (현재 로컬 IP, 포트 50313)
	const char* serverIP = "127.0.0.1";
	const int port = 50313;

	std::wstring connectMsg;

	// 서버 연결 시도; connectToServer는 resultMessage를 내부에서 설정합니다.
	if (!connectToServer(sock, serverIP, port, &connectMsg)) {
		*message = connectMsg;
		WSACleanup();
		return JS_FALSE;
	}

	*message = L"서버에 성공적으로 연결되었습니다.";
	return JS_TRUE;
}
