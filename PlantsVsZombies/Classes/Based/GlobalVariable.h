#pragma once
#include "AudioEngine.h"
#include "ui/CocosGUI.h"
#include "UserInformation.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#define VIRTUAL3D 0
#define GAME_FONT_NAME_1 "resources/fonts/GameFont.ttf"
#define GAME_FONT_NAME_2 "resources/fonts/arial.ttf"
#define MYRELEASE 0

using namespace std;
using namespace cocos2d;
using namespace cocos2d::experimental;

class Global
{
public:
	static Global* getInstance();
	int changeBgMusic(const std::string& _musicName, bool _loop);
	void stopMusic();
	void resumeMusic();
	void resumeProhibit();

public:
	int prohibitId;
	UserInformation* userInformation;

CC_CONSTRUCTOR_ACCESS:
	Global();
	~Global();

private:
	static Global* _instance;    /* µ¥Àý */
};
