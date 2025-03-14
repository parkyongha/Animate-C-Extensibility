#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>

extern "C" {
#include "jsapi.h"

	// Wide ���ڿ��� UTF-8 ���ڵ����� ��ȯ�ϴ� �Լ� ����
	std::string WideStringToUTF8(const std::wstring& wstr);

	// ������ ������ �õ��ϴ� �Լ� ����
	bool connectToServer(SOCKET& sock, const char* serverIP, int port, std::wstring* resultMessage);

	// �������� ��� ������ ������ �Լ� ����
	void communicationLoop(const char* serverIP, int port);

	// ������ ����Ǿ� �ִ��� üũ�ϴ� �Լ� ����
	bool isConnectedSocket(SOCKET sock);

	// Ŭ���̾�Ʈ ���� �Լ� ����
	// �� �Լ��� VS Code Ȯ�� ������ �����ϱ� ���� ���˴ϴ�.
	JSBool tryConnectSocketClient(std::wstring* message);
}