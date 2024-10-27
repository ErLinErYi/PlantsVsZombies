/****************************************
 *Copyright (c) 2019-2024 LZ.All Right Reserved
 *ProjectName:     PlantsVsZombies
 *Author:          LZ(ErLinEryi)
 *Date:            2019.06.13
 *LastModifyTime:  2021.02.02 14:09 2024.10.26 15.00
 *Email:           2117610943@qq.com
 ****************************************/

#include "axmol.h"
#include "LZPlantsVsZombies.h"
#include "Based/LZAppDelegate.h"
#include "Based/LZGlobalVariable.h"

#include <powrprof.h>
#pragma comment(lib, "powrprof.lib")

#define USE_WIN32_CONSOLE

using namespace ax;

// Set the preferred high performance video card
extern "C" {
_declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
}
extern "C" {
_declspec(dllexport) uint32_t AmdPowerXpressRequestHighPerformance = 0x00000001;
}

constexpr GUID scheme_high   = {0x8C5E7FDA, 0xE8BF, 0x4A96, {154, 133, 166, 226, 58, 140, 99, 92}};
constexpr GUID scheme_normal = {0x381B4222, 0xF694, 0x41F0, {150, 133, 255, 91, 178, 96, 223, 46}};
constexpr GUID scheme_low    = {0xA1841308, 0x3541, 0x4FAB, {188, 129, 247, 21, 86, 242, 11, 74}};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    // only one instance is allowed to run
    if (checkAlreadyRunning())
    {
        return false;
    }

    // check file is legitimate
    Global::checkUserDataFile();
    Global::writeComputerUniqueIdentification();

    setPowerActiveScheme();

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef USE_WIN32_CONSOLE
#    include "platform/win32/EmbedConsole.h"
#endif

    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}

bool checkAlreadyRunning()
{
    HWND hOldWnd = ::FindWindow(nullptr, _T(LZPVZNAME));
    if (hOldWnd != nullptr)
    {
        FLASHWINFO pfwi;
        pfwi.cbSize    = sizeof(pfwi);
        pfwi.uCount    = 5;
        pfwi.dwTimeout = 500;
        pfwi.hwnd      = hOldWnd;
        pfwi.dwFlags   = FLASHW_TRAY;

        FlashWindowEx(&pfwi);
        ShowWindow(hOldWnd, SW_RESTORE | SW_SHOWNORMAL);
        SetForegroundWindow(hOldWnd);
        return true;
    }
    return false;
}

void setPowerActiveScheme()
{
    GUID* id;
    PowerGetActiveScheme(nullptr, &id);
    PowerSetActiveScheme(nullptr, &scheme_high);

    scheme_default = *id;
}
