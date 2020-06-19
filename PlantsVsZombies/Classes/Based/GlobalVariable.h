#pragma once
#include "ui/CocosGUI.h"
#include "UserInformation.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#define GAME_FONT_NAME_1 "resources/fonts/GameFont.ttf"
#define GAME_FONT_NAME_2 "resources/fonts/arial.ttf"
#define MYRELEASE 0

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
	static Global* _instance;    /* µ¥Àý */
	float _sumAnimationInterval;
	int _frequence;
};
