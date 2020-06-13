/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.16
 *Email: 2117610943@qq.com
 */
 
#pragma once
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "Based/GlobalVariable.h"
#include "spine/spine-cocos2dx.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace spine;
using namespace cocos2d::experimental;

class MainMenu :public Scene
{
public:
	CREATE_FUNC(MainMenu);
	static Scene* createScene();

CC_CONSTRUCTOR_ACCESS:
	MainMenu();
	~MainMenu();
	virtual bool init();

private:
	enum class MainMenuButton
	{
		NullButton = 0,
		AdventureButton,
		SurvivalButton,
		ChallengesButton,
		VasebreakerButton
	};

	MainMenuButton checkCurInButtons();                           /* 判断鼠标光标位置*/
	void curUpdate(float time);                                   /* 鼠标光标定时器 */
	void updateUserNameOnce(float time);
	void setCloudPosition(Node* node, int ID, const Vec2& vec2);  /* 设置云的位置 */
	void playMusicBleepInGameButtons(MainMenuButton ID);          /* 播放音乐 */
	void playMusicBleepInMainButtons(int ID, const Vec2& vec2);
	void createNewUserDataFileName();                             /* 创建新存档名称*/

	/*游戏选择函数*/
	void beginAdventureGame();                                    /* 冒险模式 */
	void beginSurvivalGame();                                     /* 玩玩小游戏 */
	void beginChallengesGame();                                   /* 解谜模式 */
	void beginVasebreakerGame();                                  /* 生存模式 */

	/*回调函数*/
	void menuHelpCallBack(Ref* pSender);                          /* 帮助 */
	void menuOptionCallBack(Ref* pSender);                        /* 选项 */
	void menuQuitCallBack(Ref* pSender);                          /* 退出 */
	void menuDataCallBack(Ref* pSender);                          /* 数据 */

	/*创建函数*/
	void createMainSprite();                                      /* 创建煮主菜单精灵 */
	void createParticle();                                        /* 创建粒子特效 */
	void createSmoke(const float& Scale, const Vec2& vec2);       /* 创建烟雾粒子特效 */
	void createMouseListener();                                   /* 创建鼠标监听器 */
	void createMainButton();                                      /* 创建按钮 */
	void createClouds();                                          /* 创建云 */
	void createAnimation();                                       /* 创建动画 */
	void createFlowers();                                         /* 创建花朵 */
	void createFlowers(const float& Scale, const Vec2& vec2, const std::string& fileName, const int& ID);

	void setMouseListenerEnable(bool isEnable) { _mouse->setEnabled(isEnable); }

private:
	bool _playMusic[8] = { false };          /* 音乐是否播放 */
	Vec2 _cur;                               /* 鼠标光标位置 */
	Text* _userText;                         /* 储存用户名字 */
	Node* _userNameActionParent;             /* 用户名称动作parent */
	Menu* _menu[2];                          /* 菜单 */
	Sprite* _sprite[17];
	Sprite* _mainButton[9];                  /* 精灵 */
	MenuItemImage* _menuItem[5];             /* 菜单项 */
	EventListenerMouse* _mouse;              /* 鼠标监听 */
	Global* _global;
	Director* _director;
	LayerColor* _inputLayer;                 /* 输入屏蔽层 */
	LayerColor* _quitLayer;                  /* 退出屏蔽层 */
	LayerColor* _optionLayer;                /* 选项屏蔽层 */
};
