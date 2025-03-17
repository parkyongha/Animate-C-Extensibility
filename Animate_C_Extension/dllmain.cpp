#define MM_Initializer

extern "C"
{
#include "jsapi.h"

}
#include "SockerClient.h"

#include <string>
#include <iostream>
#include <coroutine>

// 사용자 정의 코루틴 반환 타입
struct StringTask {
	struct promise_type {
		std::string result; // 반환할 문자열을 저장

		// 코루틴 객체 생성
		auto get_return_object() {
			return StringTask{
				std::coroutine_handle<promise_type>::from_promise(*this)
			};
		}

		// 코루틴 시작 시 바로 실행
		std::suspend_always initial_suspend() {
			return std::suspend_always{};
		}

		// 코루틴 종료 후 suspend; 여기서 main에서 resume() 할 수 있음
		std::suspend_always final_suspend() noexcept {
			return	std::suspend_always{};
		}

		// 문자열을 반환할 때 호출됨
		void return_value(std::string value) {
			result = std::move(value);
		}

		// 예외 발생 시 처리
		void unhandled_exception() {
			std::terminate();
		}
	};

	// 코루틴 핸들 저장
	std::coroutine_handle<promise_type> coro;

	StringTask(std::coroutine_handle<promise_type> h) : coro(h) {}

	~StringTask() {
		if (coro)
			coro.destroy();
	}

	// 코루틴을 재개하고, 반환된 문자열을 가져옵니다.
	std::string get() {
		coro.resume();
		return coro.promise().result;
	}
};

// 간단한 코루틴 함수 예제
StringTask exampleCoroutine() {
	co_return std::string("Hello World!");
}

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

	auto task = exampleCoroutine();

	std::string taksResult = task.get();

	// 결과 메시지를 JS 문자열로 변환하여 rval에 저장
	if (!JS_StringToValue(cx,
		reinterpret_cast<unsigned short*>(const_cast<char*>(taksResult.c_str())),
		static_cast<unsigned int>(taksResult.length()),
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

//int main() {
//	auto task = exampleCoroutine();
//
//	std::string taksResult = task.get();
//
//	std::cout << taksResult;
//
//	return 0;
//}