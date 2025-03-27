#define MM_Initializer

extern "C"
{
#include "jsapi.h"
}

#include "SocketClient.h"
#include "PacketProcessor.hpp"

#include "myUtil.hpp"

#include <string>
#include <locale>
#include <codecvt>
#include <iostream>
#include <coroutine>

JSBool startSocketClient(JSContext* cx, JSObject* obj, unsigned int argc, jsval argv[], jsval* rval) {
	std::wstring socketMsg;

	std::string msg;

	// 메시지에 성공/실패 정보를 명시적으로 포함
	testAsyncSocket(msg);

	socketMsg = std::wstring(msg.begin(), msg.end());

	// 결과 메시지를 JS 문자열로 변환하여 rval에 저장
	if (!JS_StringToValue(cx,
		reinterpret_cast<unsigned short*>(const_cast<wchar_t*>(socketMsg.c_str())),
		static_cast<unsigned int>(socketMsg.length()),
		rval)) {
		return JS_FALSE;
	}

	// 실패 시 JS_FALSE, 성공 시 JS_TRUE를 반환
	return JS_TRUE;
}

JSBool getMessage(JSContext* cx, JSObject* obj, unsigned int argc, jsval argv[], jsval* rval) {

	// 문자열의 길이를 저장할 변수 선언
	unsigned int length = 0;

	// argv[0]를 unsigned short* 형태의 문자열로 변환
	unsigned short* jsStr = JS_ValueToString(cx, argv[0], &length);

	if (!jsStr) {
		return JS_FALSE;
	}

	wchar_t* cStr = reinterpret_cast<wchar_t*>(jsStr);

	auto& data = PacketProcessor::getInstance().getParsedDataByName(cStr);

	JSObject* jsArray = JS_NewArrayObject(cx, 0, nullptr);

	for (unsigned int i = 0; i < data.size(); i++) {
		const std::string& str = data[i];

		std::wstring wstr = std::wstring(data[i].begin(), data[i].end());

		// wstring -> unsigned short*
		unsigned short* item = wstr2ushort(wstr);
		unsigned int itemLen = static_cast<unsigned int>(wstr.length());

		jsval val;

		if (!JS_StringToValue(cx, item, itemLen, &val))
			return JS_FALSE;

		if (!JS_SetElement(cx, jsArray, i, &val))
			return JS_FALSE;
	}

	std::wstring testErrWstr = L"TEST::테스트";

	unsigned short* testErr = wstr2ushort(testErrWstr);
	unsigned int testErrLen = static_cast<unsigned int>(testErrWstr.length());

	JS_ReportError(cx, testErr, testErrLen);

	*rval = JS_ObjectToValue(jsArray);

	return JS_TRUE;
}

JSBool _sendMessage(JSContext* cx, JSObject* obj, unsigned int argc, jsval argv[], jsval* rval) {
	sendMessageToServer("str");

	// 문자열의 길이를 저장할 변수 선언
	unsigned int length = 0;

	// 문자열 얻기 (UTF-16 형식)
	unsigned short* jsStr = JS_ValueToString(cx, argv[0], &length);

	if (!jsStr || length == 0) {
		std::wstring errMsg = L"Invalid string argument.";
		JS_ReportError(cx, wstr2ushort(errMsg), 0);
		return JS_FALSE;
	}

	// UTF-16 문자열 확보 (wchar_t 호환)
	std::wstring ws(jsStr, jsStr + length);

	// UTF-8 문자열로 변환
	std::string str = WStringToUTF8(ws);

	sendMessageToServer("str");

	return JS_TRUE;
}

JSBool test(JSContext* cx, JSObject* obj, unsigned int argc, jsval argv[], jsval* rval) {

	sendMessageToServer("str");

	return JS_TRUE;
}

MM_STATE

void MM_Init() {
	JS_DefineFunction(reinterpret_cast<unsigned short*>(const_cast<wchar_t*>(L"startSocketClient")), startSocketClient, 0);
	JS_DefineFunction(reinterpret_cast<unsigned short*>(const_cast<wchar_t*>(L"getMessage")), getMessage, 1);
	JS_DefineFunction(reinterpret_cast<unsigned short*>(const_cast<wchar_t*>(L"sendMessage")), test, 1);
}

//int main() {
//
//	std::string msg;
//	testAsyncSocket(msg);
//	std::cout << msg;
//
//	std::cout << "소켓 연결 후 대기 중..." << '\n';
//
//	std::string str = "Test Hello World\n";
//
//	sendMessage(str);
//
//	std::cin.get();
//
//	return 0;
//}