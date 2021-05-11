/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Email: 2117610943@qq.com
 */

#pragma once
#include "ui/CocosGUI.h"
#include "LZUserInformation.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#define USEFONT(__FONT__) Global::getInstance()->userInformation->getGameText().find(__FONT__)->second->text
#define GAME_FONT_NAME_1 USEFONT("TTF字体名称1")
#define GAME_FONT_NAME_2 USEFONT("TTF字体名称2")
#define GAME_FONT_NAME_3 USEFONT("TTF字体名称3")

#define MYRELEASE 1

#define COUNTPLAYERS                                                                                               \
ShellExecute(0, L"open", L"iexplore.exe", L"https://gitlz.gitee.io/lz/CountPlayer.html", NULL, SW_SHOWMINIMIZED);  \
runAction(Sequence::create(DelayTime::create(1.f),                                                                 \
	CallFunc::create([]()                                                                                          \
		{                                                                                                          \
			ShowWindow(FindWindow(L"IEFrame", NULL), SW_HIDE);                                                     \
		}), DelayTime::create(5.f),                                                                                \
	CallFunc::create([]()                                                                                          \
		{                                                                                                          \
			SendMessage(FindWindow(L"IEFrame", NULL), WM_CLOSE, NULL, NULL);                                       \
		}), nullptr)); 


using namespace std;
using namespace cocos2d;

class Global
{
public:
	static Global* getInstance();
	void resumeProhibit();
	void checkAnimationInterval();

public:
	int prohibitId;
	UserInformation* userInformation;

private:
	Global();
	~Global();

private:
	static Global* _instance;    /* 单例 */
	float _sumAnimationInterval;
	int _frequence;
};
