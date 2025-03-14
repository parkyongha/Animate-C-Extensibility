
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

// Wide ���ڿ��� UTF-8 ���ڵ����� ��ȯ�ϴ� �Լ�
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

	// IP �ּ� ��ȯ ���� ó��
	if (InetPtonA(AF_INET, serverIP, &serverAddr.sin_addr) != 1) {
		std::wstringstream ws;
		// char*�� std::wstring���� ��ȯ (����IP�� ASCII�� ����)
		ws << L"�߸��� IP �ּ��Դϴ�: " << std::wstring(serverIP, serverIP + std::strlen(serverIP)) << L"\n";
		*message = ws.str();
		return false;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET) {
		std::wstringstream ws;
		ws << L"���� ���� ����, ���� �ڵ�: " << WSAGetLastError() << L"\n";
		*message = ws.str();
		return false;
	}

	if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::wstringstream ws;
		ws << L"���� ���� ����, ���� �ڵ�: " << WSAGetLastError() << L"\n";
		*message = ws.str();
		closesocket(sock);
		return false;
	}

	std::wstringstream ws;
	ws << L"������ ���������� ����Ǿ����ϴ�.";
	*message = ws.str();

	return true;
}

// �������� ��� ������ ������ �Լ�
void communicationLoop(const char* serverIP, int port) {
	WSADATA wsaData;
	// Winsock �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup ����\n");
		return;
	}

	SOCKET sock;

	// ������ �޽��� ����
	// Wide ���ڿ��� �޽����� ���� (UTF-16
	std::wstring wideMsg = L"call:someFunction:HelloFromAnimate �ȳ�";
	// UTF-8�� ��ȯ
	std::string msg = WideStringToUTF8(wideMsg);

	int bytesSent = send(sock, msg.c_str(), (int)msg.length(), 0);
	if (bytesSent == SOCKET_ERROR) {
		printf("�޽��� ���� ����, ���� �ڵ�: %d\n", WSAGetLastError());
	}
	else {
		printf("�޽��� ���� ����: %s\n", msg.c_str());
	}

	// �����κ��� ���� ����
	char buffer[512];
	int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);

	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';  // ���ڿ� ����

		// (1) UTF-8 �� WideChar ��ȯ
		wchar_t wbuffer[512];
		int wlen = MultiByteToWideChar(CP_UTF8, 0, buffer, -1, wbuffer, 512);
		if (wlen > 0) {
			wbuffer[wlen] = L'\0';
			// (2) WideChar ���ڿ��� wprintf�� ���
			wprintf(L"���� ����: %s\n", wbuffer);
		}
		else {
			wprintf(L"MultiByteToWideChar ��ȯ ����\n");
		}
	}
	else {
		wprintf(L"recv ����, ���� �ڵ�: %d\n", WSAGetLastError());
	}

	// ���� ���� �� Winsock ����
	closesocket(sock);
	WSACleanup();
}

bool isConnectedSocket(SOCKET sock) {
	// 0����Ʈ ������ �õ��մϴ�.
	int ret = send(sock, "", 0, 0);

	if (ret == SOCKET_ERROR) {
		int err = WSAGetLastError();

		return false;
	}

	return true;
}
JSBool tryConnectSocketClient(std::wstring* message) {
	// ���� ������ ����� ����(sock)�� �̹� ����Ǿ� �ִ��� Ȯ��
	if (isConnectedSocket(sock)) {
		*message = L"�̹� ������ ����Ǿ� �ֽ��ϴ�.";
		return JS_TRUE;
	}

	WSADATA wsaData;
	int startupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (startupResult != 0) {
		std::wstringstream ws;
		ws << L"WSAStartup ����, ���� �ڵ�: " << startupResult;
		*message = ws.str();
		return JS_FALSE;
	}

	// ���� ���� ���� (���� ���� IP, ��Ʈ 50313)
	const char* serverIP = "127.0.0.1";
	const int port = 50313;

	std::wstring connectMsg;

	// ���� ���� �õ�; connectToServer�� resultMessage�� ���ο��� �����մϴ�.
	if (!connectToServer(sock, serverIP, port, &connectMsg)) {
		*message = connectMsg;
		WSACleanup();
		return JS_FALSE;
	}

	*message = L"������ ���������� ����Ǿ����ϴ�.";
	return JS_TRUE;
}
