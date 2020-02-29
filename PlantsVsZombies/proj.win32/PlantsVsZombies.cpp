/****************************************
 *Copyright (c) 2019 LZ.All Right Reserved
 *ProjectName:     PlantsVsZombies
 *Author:          LZ(ErLinEryi)
 *Date:            2019.06.13
 *LastModifyTime:  2020.01.10 18:30
 *Email:           2117610943@qq.com
 ****************************************/

#include "cocos2d.h"
#include "AppDelegate.h"
#include "PlantsVsZombies.h"

USING_NS_CC;

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
