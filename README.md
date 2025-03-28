# Animate Extensibility DLL

이 프로젝트는 C++로 작성된 DLL을 통해 **Adobe Animate**에서 C-Level Extensibility 기능을 확장하는 예제입니다. Animate 내부에서 동작하는 **JSFL 스크립트**가 C/C++로 구현된 기능(예: 소켓 통신)을 직접 호출할 수 있도록 설계되었습니다.

## 주요 기능

- **C/C++ 기반 DLL 구현**  
  - `main.cpp`로부터 시작해 Animate에 노출할 내보내기 함수(예: `MM_Init()`)를 등록합니다.  
  - `JSNative` 함수를 통해 Animate(JSFL)에서 `Animate_C_Extension.함수명()` 형태로 호출할 수 있습니다.

- **소켓 통신 기능**  
  - 내부적으로 **Winsock**을 사용하여 TCP 소켓 통신을 수행합니다.  
  - `SocketClient` 모듈에서 서버에 연결하고, 수신 받은 데이터를 `PacketProcessor`가 파싱해 저장/관리합니다.
  - Node.js 측(`server.js`)에서 `net.createServer()`로 50313 포트에 대해 Listen을 수행하며, C++ DLL이 접속합니다.

- **C/C++와 JSFL 간의 데이터 교환**  
  - `JS_ValueToString`, `JS_StringToValue` 등의 C-Level Extensibility API로, JSFL 스크립트와 문자열을 주고받습니다.  
  - 수신된 데이터는 `myUtil` 모듈을 통해 UTF-8 ↔ UTF-16 등으로 변환 처리될 수 있습니다.

## 구성 요소

- **main.cpp**  
  - Animate가 DLL을 로드하면, `MM_Init()`를 통해 JSNative 함수를 등록해 JSFL 스크립트와 연동합니다.  

- **SocketClient.h / SocketClient.cpp**  
  - TCP 소켓 생성, 서버 연결/해제, 데이터 송수신을 담당합니다.  
  - 별도의 쓰레드(`receiveLoop`)에서 서버의 메시지를 받아 `PacketProcessor`로 전달합니다.  

- **PacketProcessor.h / PacketProcessor.cpp**  
  - 소켓을 통해 들어온 문자열 데이터를 구분자(`:` 및 `,`)로 파싱하여 저장/관리합니다.  
  - 필요한 경우 Animate 측(혹은 JSFL 측)으로 전달해 활용할 수 있게 합니다.  

- **myUtil.h / myUtil.cpp**  
  - `std::wstring` ↔ `std::string(UTF-8)` 등 문자열 변환을 담당하는 유틸 기능 모음입니다.  

## 빌드 및 배포

1. **빌드 환경**
   - Visual Studio 2022 (C++20 이상)
   - 64비트 DLL(Animate 2024는 64비트이므로)

2. **DLL 배치 위치**
   - 빌드된 DLL 파일을 아래 경로에 배치해야 Animate에서 인식됩니다:
     ```
     C:\Users\USER\AppData\Local\Adobe\Animate 2024\en_US\Configuration\External Libraries\
     ```
   - Animate 시작 시, 이 폴더의 DLL을 검색해 확장 기능을 자동으로 로드합니다.

## 동작 방식

1. Animate가 시작될 때, DLL을 읽어 `MM_Init()` 함수 등 내보낸(export) 함수를 확인합니다.  
2. `MM_Init()`에서는 Animate(JSFL) 스크립트에서 호출할 수 있는 C++ 함수를 등록합니다.  
3. JSFL 스크립트 내에서 예:
   ```js
   Animate_C_Extension.startSocketClient();
   ``` 
   와 같이 호출하면, DLL 내의 C++ 함수가 실행되고, 소켓 연결 등 네이티브 로직을 수행합니다.