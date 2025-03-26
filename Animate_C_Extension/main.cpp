#define MM_Initializer

extern "C"
{
#include "jsapi.h"
}

#include "SocketClient.h"
#include "PacketProcessor.hpp"

#include "myUtil.hpp"

#include <string>
#include <iostream>
#include <coroutine>

JSBool startSocketClient(JSContext* cx, JSObject* obj, unsigned int argc, jsval argv[], jsval* rval) {
	std::wstring socketMsg;

	std::string msg;

	// 메시지에 성공/실패 정보를 명시적으로 포함
	testAsyncSocket(msg);

	socketMsg = std::wstring(msg.begin(), msg.end());
	/*if () {
		socketMsg = L"성공: " + socketMsg;
	}
	else {
		socketMsg = L"오류: " + socketMsg;
	}*/

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
	std::wstring testErrWstr = L"TEST :: 테스트";

	auto testErr = wstr2ushort(testErrWstr);
	unsigned int testErrLen = static_cast<unsigned int>(testErrWstr.length());

	JS_ReportError(cx, testErr, testErrLen);

	*rval = JS_ObjectToValue(jsArray);

	return JS_TRUE;
}

JSBool sendMessage(JSContext* cx, JSObject* obj, unsigned int argc, jsval argv[], jsval* rval) {
	return JS_FALSE;
}

MM_STATE

void MM_Init() {
	JS_DefineFunction(reinterpret_cast<unsigned short*>(const_cast<wchar_t*>(L"startSocketClient")), startSocketClient, 0);
	JS_DefineFunction(reinterpret_cast<unsigned short*>(const_cast<wchar_t*>(L"getMessage")), getMessage, 1);
	JS_DefineFunction(reinterpret_cast<unsigned short*>(const_cast<wchar_t*>(L"sendMessage")), sendMessage, 2);
}

//int main() {
//	//std::wstring wmsg;
//	//tryConnectSocketClient(wmsg);
//	//std::wcout << wmsg;
//
//	std::string msg;
//	testAsyncSocket(msg);
//	std::cout << msg;
//
//	return 0;
//}