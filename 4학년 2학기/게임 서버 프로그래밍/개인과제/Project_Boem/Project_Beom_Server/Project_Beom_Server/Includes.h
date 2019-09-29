#pragma once

// 윈도우 Warning 예외처리
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <iostream>
#include <vector>

#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#include "Const.h"
#include "Struct.h"
#include "Macro.h"
#include "Enum.h"

#include "ChessManager.h"