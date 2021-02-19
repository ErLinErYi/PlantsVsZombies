/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.6.25
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"
#include "Based/LZBGlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::experimental;

class VideoPlayer;

class GameEasterEggs :public Scene
{
public:
	CREATE_FUNC(GameEasterEggs);

CC_CONSTRUCTOR_ACCESS:
	GameEasterEggs();
    ~GameEasterEggs();
	virtual bool init();

private:
    enum class State
    {
        SWIMMING,
        SWIMMING_TO_HURT,
        HURT,
        HURT_TO_SWIMMING,
    };

	void createEggText();
	void musicCallBack();
    void backButton();
    void playVideo();
    void createSlider();

private:
	Global*    _global;
	Label*     _eggstext;
    string     _fileName;
    Slider*    _slider;
    VideoPlayer* _pPlayer;
	int        _audioId;
};
