# Animate Extensibility DLL

이 프로젝트는 C++로 작성된 DLL을 통해 Adobe Animate의 C-Level Extensibility 기능을 확장하는 예제입니다. 이 DLL은 Animate에서 JSFL 스크립트를 통해 호출할 수 있는 함수를 내보내어, Animate 내부에서 C/C++ 기반의 기능(예: 소켓 통신)을 실행할 수 있도록 설계되었습니다.

## 주요 기능

- **C/C++ 기반 DLL 구현**  
  - C++로 DLL을 작성하여 Animate와의 연동을 위한 내보내기 함수(`MM_InitWrapper`)를 제공합니다.
  - 외부에서 호출 가능한 JSNative 함수들을 등록하여, Animate(JSFL) 스크립트에서 `Animate_C_Extension.함수명()` 형태로 호출할 수 있습니다.

- **소켓 통신 기능**  
  - DLL 내에서 Winsock을 사용하여 서버와의 소켓 통신 기능을 구현합니다.
  - 예를 들어, VS Code 확장에서 구동되는 서버와 연결하여 실시간 데이터를 주고받을 수 있습니다.
  - 연결 상태를 확인하기 위해 `send()`를 이용한 간단한 연결 체크 함수를 포함하고 있습니다.

- **C/C++와 JSFL 간 인터페이스**  
  - Animate의 C-Level Extensibility API를 이용해, C/C++ 코드와 JSFL 간의 데이터 및 함수 호출을 중계합니다.
  - `JS_ValueToString`, `JS_StringToValue` 등의 API를 사용해 JavaScript 값과 C++ 문자열 간의 변환을 지원합니다.

## 구성 요소

- **jsapi.h**  
  Animate의 C-Level Extensibility 인터페이스를 위한 타입 및 함수 선언을 포함합니다.  
  이 파일은 JSNative 함수의 시그니처와, Animate와의 링케이지에 필요한 매크로들을 정의합니다.

- **dllmain.cpp**  
  DLL의 진입점(`MM_InitWrapper`) 및 확장 함수 등록 로직을 구현합니다.  
  여기서 `MM_Init()`를 호출하여 Animate에 내보낼 함수(예: `helloFunction`, `startSocketClient`)들을 등록합니다.

- **SockerClient.h / SocketClient.cpp**  
  Winsock을 활용한 소켓 통신 기능을 구현한 파일들입니다.  
  이 모듈은 서버와의 연결, 연결 상태 체크, 메시지 송수신 등을 담당합니다.

## 빌드 및 배포

- **빌드 환경:**  
  - Visual Studio 2022 (C++17 이상)
  - 64비트 DLL (Animate 2024는 64비트 애플리케이션입니다.)

- **배포 경로:**  
  최종 빌드된 DLL 파일은 아래 경로에 배치해야 합니다.
  ```
  C:\Users\USER\AppData\Local\Adobe\Animate 2024\en_US\Configuration\External Libraries\
  ```
  Animate는 이 폴더 내의 DLL을 자동으로 스캔하여 확장을 로드합니다.

## 사용 방법

1. Animate가 시작될 때, Animate는 해당 DLL을 로드하고 `MM_InitWrapper` 심볼을 찾습니다.
2. `MM_InitWrapper` 내부에서는 `MM_Init()`를 호출하여 JSNative 함수들을 Animate에 등록합니다.
3. JSFL 스크립트 내에서 등록된 함수(예: `Animate_C_Extension.startSocketClient()`)를 호출하면, DLL 내의 함수가 실행되고, 소켓 통신 등의 기능이 수행됩니다.

## 참고 사항

- C++와 C 코드를 함께 사용하는 경우, 함수 선언은 `extern "C"`로 감싸 C 링케이지를 보장해야 합니다.
- DLL 내 함수의 이름은 Animate와 JSFL 스크립트에서 정확히 일치해야 하므로, 대소문자 및 철자에 주의해야 합니다.
- Dependency Walker 등 도구를 사용해, DLL이 올바르게 내보내진(export) 함수들을 확인할 수 있습니다.
