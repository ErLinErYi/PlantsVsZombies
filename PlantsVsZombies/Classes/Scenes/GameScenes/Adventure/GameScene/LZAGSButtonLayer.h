/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "../SelectPlantsScene/LZASPSControlLayer.h"
#include "Based/LZUserWinRequirement.h"
#include "Based/LZGameType.h"
#include "Based/LZLevelData.h"
#include "Plants/LZPlants.h"

using namespace cocos2d;
using namespace cocos2d::ui;

struct MouseSelectImage /* 鼠标选择的植物 */
{
	MouseSelectImage():Iamge(nullptr), isSelectPlants(false), isSelectShovel(false){}
	Sprite* Iamge;                    /* 图片 */
	PlantsType selectPlantsId;        /* 所选植物编号 */
	bool isSelectPlants;              /* 是否选择了植物 */
	bool isSelectShovel;              /* 是否选择了铲子 */
};

enum class GSLayerButton
{
	stopButton = 1,
	accelerateButton,
	decelerateButton,
	nextWaveButton,
	recoverySunButton,
	recoveryCoinButton
};

class GSButtonLayer :public SPSControlLayer
{
public:
	CREATE_FUNC(GSButtonLayer);

	/**
	 *添加到主场景中
	 */
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	GSButtonLayer();
	~GSButtonLayer();
	virtual bool init();

protected:
	virtual Button* createButton(const string& normal, const string& select, const Vec2& position, GSLayerButton buttonName, const bool isFlippedX = false);
	virtual void createKeyBoardListener();
	virtual void createQuitDialog();
	virtual void createRequirementButton();
	virtual void createRequirementLayer();
	virtual void controlAccelerateScheduler();
	virtual void controlDecelerateScheduler();
	virtual void showSeedBank();
	virtual void showShovelBank();
	virtual void showNextWaveButton();
	virtual void nextWaveButtonControl();
	virtual void createPlantsCard();
	virtual void createJumpLevelButton();
	virtual void recoverySunControl();
	virtual void recoveryCoinControl();
	virtual ProgressTimer* createProgressTimer(Button* button, const float _time, const int from, const unsigned int& id);
	
public:
	MouseSelectImage* mouseSelectImage;                            // 鼠标选择
	PlantsInformation::PlantsCards plantsCards[PLANTSNUMBERS];     // 植物卡片
	Button* nextWaveButton;                                       // 下一波按钮
	
protected:
	Global* _global;                                     // 全局变量
	Director* _director;                                 // 导演单例
	Layer* _quitLayer;                                   // 退出屏蔽层
	Button* _accelerateButton;                           // 加速按钮
	Button* _decelerateButton;                           // 减速按钮
	OpenLevelData* _openLevelData;
};

