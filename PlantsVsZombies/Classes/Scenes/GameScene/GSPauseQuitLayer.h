/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Scenes/MainMenuScene/OptionsSence.h"

using namespace cocos2d;

enum class PauseQuitLayer_Button
{
	查看图鉴 = 0,
	从新开始,
	退出游戏,
	按键说明,
	返回游戏
};

class GSPauseQuitLayer :public OptionsMenu
{
public:
	CREATE_FUNC(GSPauseQuitLayer);
	static Layer* addLayer();
	static void pauseLayer();
	static void resumeLayer();
	static int getPauseNumbers();
	
CC_CONSTRUCTOR_ACCESS:
	GSPauseQuitLayer();
	~GSPauseQuitLayer();
	virtual bool init();

protected:
	virtual void createButton(const Vec2& vec2, const std::string name, PauseQuitLayer_Button type);
	virtual void createDialog() override;
	virtual void popSceneAnimation();

private:
	void showPrompt();
	void openHandBook();
	void setRestart();
	void setQuitGame();
	void keyDescription();
	void returnGame();

protected:
	EventListenerTouchOneByOne* _touchListener;

private:
	LayerColor* _promptLayer;
	static int _pauseNumbers;
	static string _layerName[6];
	char* _levelName;
};