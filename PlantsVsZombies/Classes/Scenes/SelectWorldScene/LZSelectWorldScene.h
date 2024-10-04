/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.15
 *Modify: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Based/LZGlobalVariable.h"

#define WorldNumbers 2

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;

class SelectWorldScene :public Scene
{
public:
	CREATE_FUNC(SelectWorldScene);
	static Scene* createScene();             /* 创建场景 */
	static void createGalaxy(Node* node);

CC_CONSTRUCTOR_ACCESS:
	SelectWorldScene();
	virtual bool init() override;

private:
	void showTimeTravelAnimation();
	void showBackground();
	void createScrollView();
	void showDifferentWorlds();
	void createMouseListener();
    void createGoBack();
	void createSelectDifficulty();
	void modeButtonCallBack(RadioButton* button, int worldId);
	void modeButtonText(RadioButton* button, const string& modeName, const Vec2& position, const Color4B& color1, const Color4B& color2);
	int checkWorldUnlock();
	int checkModeUnlock(int id = 1);
	
private:
	Size _backgroundSize;
	float _selectWorldPosition;
	cocos2d::ui::ScrollView* _scrollView;
	Button* _world[WorldNumbers + 1];
	Global* _global;
};