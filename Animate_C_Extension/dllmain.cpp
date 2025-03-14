#define MM_Initializer

extern "C"
{
#include "jsapi.h"

}
#include "SockerClient.h"

#include <string>

JSBool StartSockerClient(JSContext* cx, JSObject* obj, unsigned int argc, jsval argv[], jsval* rval) {
    std::wstring socketMsg;

    JSBool result = tryConnectSocketClient(&socketMsg);

    // 메시지에 성공/실패 정보를 명시적으로 포함
    if (result == JS_TRUE) {
        socketMsg = L"성공: " + socketMsg;
    }
    else {
        socketMsg = L"오류: " + socketMsg;
    }

    // 결과 메시지를 JS 문자열로 변환하여 rval에 저장
    if (!JS_StringToValue(cx,
        reinterpret_cast<unsigned short*>(const_cast<wchar_t*>(socketMsg.data())),
        static_cast<unsigned int>(socketMsg.length()),
        rval)) {
        return JS_FALSE;
    }

    // 실패 시 JS_FALSE, 성공 시 JS_TRUE를 반환
    return JS_TRUE;
}

MM_STATE

void MM_Init() {
	JS_DefineFunction(reinterpret_cast<unsigned short*>(const_cast<wchar_t*>(L"startSocketClient")), StartSockerClient, 0);
}
