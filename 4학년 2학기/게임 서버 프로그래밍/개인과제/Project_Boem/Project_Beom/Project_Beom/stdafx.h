// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.

#include <time.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <random>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
using namespace std;

// 서버
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

// 사용자 헤더
#include "Include.h"

#include "FrameManager.h"
#include "TimerManager.h"
#include "KeyManager.h"
#include "BmpManager.h"
#include "AbstractFactory.h"
#include "SceneManager.h"
#include "ChessManager.h"
#include "NetworkManager.h"
#include "WinCallbackManager.h"

// TransparentBlt 라이브러리
#pragma comment(lib, "msimg32.lib")