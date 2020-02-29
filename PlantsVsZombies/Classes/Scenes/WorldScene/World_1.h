/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.18
 *Modify: 2020.1.28
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

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;

class World_1 :public Scene
{
public:
	CREATE_FUNC(World_1);
	static Scene* createScene();  

CC_CONSTRUCTOR_ACCESS:
	World_1();
	~World_1(){}
	virtual bool init(); 

private:
	void readWorldLevel();
	void createGoBack();
	void createBackground();
	void createScrollView();
	void addScrollView();
	void createButtonListener(ui::Button* button, const int& ID);
	void createButton(Node* node, const std::string& name, const Vec2& position);
	Sprite* createSprite(const std::string& name, const Vec2& position, const float& scale, const int& zorder, const float& speed, bool IsFlipped = false);
	Sprite* createSprite(Node* node, const std::string& name, const Vec2& position, const float& scale, const int& zorder,bool IsFlipped = false);
	void createMouseListener();

private:
	Global* _global;
	Director* _director;
	ParallaxNode* _parallax;
	Size _backgroundSize;
	ui::ScrollView* _scrollView;
	int _level;
	float _worldPosition;                 /* 世界初始位置 */
	float _worldRollingDistance;          /* 计算鼠标滚动距离 */
	list<Button*> _levelButton;
};