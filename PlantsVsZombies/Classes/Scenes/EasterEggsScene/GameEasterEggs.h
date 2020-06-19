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
#include "Based/GlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::experimental;

class VideoPlayer;

class GameEasterEggs :public Scene
{
public:
	static Scene* createScene();             /* ´´½¨³¡¾° */
	CREATE_FUNC(GameEasterEggs);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);

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

	void addtortoiseRotate();
    void addtortoiseStraight();
	void rotateUpdate(float delta);
    void straightUpdate(float delta);
    void reachEndCallBack();
    void renewCallBack();
	void createEggText();
	void musicCallBack();
    void playVideo();
    void createSlider();

private:
	Global*    _global;
	Sprite3D*  _sprite;
    Sprite3D*  _sprite2;
	Label*     _eggstext;
    Action*    _swim;
    Animate3D* _hurt;
    State      _state;
    MoveTo*    _moveAction;
    string     _fileName;
    Slider*    _slider;
    VideoPlayer* _pPlayer;

	float      _arcSpeed;
	float      _radius;
	float      _accAngle;
    float      _elapseTransTime;
	int        _audioId;
};
