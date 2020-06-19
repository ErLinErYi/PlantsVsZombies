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
	static void setPopEnter(const bool isPopEnter);

CC_CONSTRUCTOR_ACCESS:
	World_1();
	~World_1();
	virtual bool init(); 
	virtual void onEnter();

protected:
	virtual void readWorldLevel();
	virtual void setLevelVisible(Node* node);
	virtual void createButtonListener(ui::Button* button, const int& ID);
	virtual ui::Button* createButton(Node* node, const std::string& name, const Vec2& position);
	virtual void createScrollView();
	virtual void createGoBack();
	void showLevels();
	void createBackground();
	void addScrollView(const int id);
	Sprite* createSprite(const std::string& name, const Vec2& position, const float& scale, const int& zorder, const float& speed, bool IsFlipped = false);
	Sprite* createSprite(Node* node, const std::string& name, const Vec2& position, const float& scale, const int& zorder,bool IsFlipped = false);
	void createMouseListener();
	void playProhibitMusic(Button* button);
	void playLevelGameAndCaveThings(const int id);

protected:
	Global* _global;
	Director* _director;
	ParallaxNode* _parallax;
	Size _backgroundSize;
	ui::ScrollView* _scrollView;
	int _level;
	float _worldPosition;                 /* 世界初始位置 */
	float _worldRollingDistance;          /* 计算鼠标滚动距离 */
	list<Button*> _levelButton;

private:
	static bool _isPopEnter;
};
