/****************************************
 *Copyright (c) 2019-2020 LZ.All Right Reserved
 *ProjectName:     PlantsVsZombies
 *Author:          LZ(ErLinEryi)
 *Date:            2019.06.13
 *LastModifyTime:  2020.06.17 8:34
 *Email:           2117610943@qq.com
 ****************************************/

#include "cocos2d.h"
#include "Based/AppDelegate.h"
#include "PlantsVsZombies.h"

USING_NS_CC;

// Set the preferred high performance video card
extern "C" { _declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001; }
extern "C" { _declspec(dllexport) uint32_t AmdPowerXpressRequestHighPerformance = 0x00000001; }

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    HANDLE hMutex = ::CreateMutex(NULL, TRUE, L"LZPVZGAME");
    if (hMutex != NULL)
    {
        if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
            CloseHandle(hMutex);
            hMutex = NULL;
            MessageBoxW(nullptr, L"此程序已经启动，禁止多次启动此程序!", L"error", MB_OK);

            return 0;
        }
    }

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
