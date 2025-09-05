#pragma once
#include "MyWindow.h"

class Tools {

public:
    static void LogErrorMsg(DWORD errorCode)
    {
        LPVOID lpMsgBuf;
        DWORD bufLen = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&lpMsgBuf,
            0,
            nullptr);

        if (bufLen)
        {
            std::string errorMsg((LPSTR)lpMsgBuf, bufLen);
            LocalFree(lpMsgBuf);
            
            OutputDebugStringA(("System Error " + std::to_string(errorCode) + ": " + errorMsg).c_str());
        }
        else
        {
            OutputDebugStringA("Failed to get error message.\n");
        }
    }
 
};

