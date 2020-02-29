#pragma once
#include "AudioEngine.h"
#include "ui/CocosGUI.h"
#include "UserInformation.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace std;
using namespace cocos2d;
using namespace cocos2d::experimental;

class Global
{
public:
	static Global* getInstance();
	int changeBgMusic(const std::string& _musicName, bool _loop);

public:
	UserInformation* userInformation;

CC_CONSTRUCTOR_ACCESS:
	Global();
	~Global();

private:
	static Global* _instance;    /* µ¥Àý */
};
