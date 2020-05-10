/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Scenes/SelectPlantsScene/SPSControlLayer.h"
#include "Based/UserWinRequirement.h"
#include "Based/GameType.h"
#include "Based/LevelData.h"
#include "Plants/Plants.h"

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

class PlantsInformation
{
public:
	struct PlantsCards
	{
		PlantsCards():
			timeBarIsFinished(false)
		{}
		Button* plantsCards;            /* 卡牌 */
		Text* plantsCardText;           /* 卡牌文字 */
		ProgressTimer* progressTimer;   /* 倒计时 */
		int plantsNeedSunNumbers;       /* 所需阳光 */
		int tag;                        /* 编号 */
		bool timeBarIsFinished;         /* 倒计时是否完成 */
	};

	/* 植物冷却时间 */
	float PlantsCoolTime[13] =
	{
		/*向日葵*/  7.5f, /*豌豆射手    */  7.5f, /*坚果    */  30,   /*樱桃炸弹*/     35,
		/*土豆雷*/  30,   /*卷心菜投手  */  7.5f, /*火炬树桩*/  7.5f, /*地刺    */     7.5f,
		/*大蒜  */  10,   /*火焰豌豆射手*/  10,   /*火爆辣椒*/  35,   /*强酸柠檬射手*/ 7.5f,
		/*离子缘*/  7.5f
	};
	/* 植物所需阳光 */
	int PlantsNeedSunNumbers[13] =
	{
		/*向日葵*/ 50, /*豌豆射手    */ 100, /*坚果    */ 50,  /*樱桃炸弹*/     150,
		/*土豆雷*/ 25, /*卷心菜投手  */ 100, /*火炬树桩*/ 175, /*地刺    */     100,
		/*大蒜  */ 50, /*火焰豌豆射手*/ 200, /*火爆辣椒*/ 150, /*强酸柠檬射手*/ 125,
		/*离子缘*/ 350
	};

	/* 植物剩余冷却时间 */
	struct PlantsCardFileData
	{
		static float PlantsSurPlusCoolTime[13];
		static float PlantsSurPlusPrecent[13];
	};
};

enum class GSLayerButton
{
	stopButton = 1,
	accelerateButton,
	decelerateButton
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

private:
	Button* createButton(const string& normal, const string& select, const Vec2& position, GSLayerButton buttonName, const bool isFlippedX = false);
	void createKeyBoardListener();
	void createQuitDialog();
	void createRequirementButton();
	void createRequirementLayer();
	void controlAccelerateScheduler();
	void controlDecelerateScheduler();
	void showSeedBank();
	void showShovelBank();
	void createPlantsCard();
	ProgressTimer* createProgressTimer(Button* button, const float _time, const int from, const unsigned int& id);
	
public:
	MouseSelectImage* mouseSelectImage;                 // 鼠标选择
	PlantsInformation* plantsInformation;               // 植物属性
	PlantsInformation::PlantsCards plantsCards[20];     // 植物卡片
	
private:
	Global* _global;                                     // 全局变量
	Director* _director;                                 // 导演单例
	Layer* _quitLayer;                                   // 退出屏蔽层
	Button* _accelerateButton;                           // 加速按钮
	Button* _decelerateButton;                           // 减速按钮
	OpenLevelData* _openLevelData;
};

