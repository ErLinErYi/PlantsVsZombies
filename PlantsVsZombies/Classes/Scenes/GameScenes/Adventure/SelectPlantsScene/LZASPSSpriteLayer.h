/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"

#include "Based/LZGlobalVariable.h"
#include "Plants/LZPlants.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace spine;

class Plants;
enum class PlantsType;

class SPSSpriteLayer : public Layer
{
public:
	CREATE_FUNC(SPSSpriteLayer);
	static void createButtonHoverEffect(Button* button);
	static void pauseButtonHoverEffect();
	static void resumeButtonHoverEffect();
	static void createPlantsText(const unsigned int& ID, const std::string& name, 
		const Vec2& vec2, const float& fontsize, Color3B color = Color3B::WHITE, bool AnchorPoint = true);
	static string selectRequirementText(map<string, LanguageTextAttribute*>& lta, PlantsType type, string str, string str1);
	virtual Text* showPlantsInformation(Button* button, bool showHeart = false);

protected:
	virtual void controlPlantCanSelect(Button* button, int priority);
	virtual void createSelectPlantsDialog();

private:
	void alreadySelectPlantsDialog();
	void alreadyHavePlantsDialog();
	void createScrollview();
	void createMouseListener();
	void createPlantsCards();
	void showRandPlantsInformation();
	Button* createButtons(const Vec2& vec2,int priority);
	void showSelectedButtonHoverEffect(EventMouse* e);
	void showPopulationButtonHoverEffect(EventMouse* e);
	void preLoadText();
	void createMoveButton(Button* button, const Vec2& vec2);
	void createAnimationAndText(PlantsType type);
	void sortPlantsCard(PlantsType type);
	void createBeginButton();
	void startGame();
	void selectPlantsCallBack(Node* node) { _selectFinished = true; }
	float calculateScrollDistance();
	float calculateScrollPrecent(float distance);
	void popUnlockDialog();
	void plantsCardButtonEvent(Button* button, Vec2 vec2);
	void plantsMoveCardButtonEvent(Button* button, Button* moveCard, Vec2 vec2, float plantCardRollingDistanceLast);
	
CC_CONSTRUCTOR_ACCESS:
	SPSSpriteLayer();
	~SPSSpriteLayer();
	virtual bool init();

public:
	static Text* animationText[PLANTSNUMBERS];        /* 动画说明 */
	static bool isPlantIsCanSelect[PLANTSNUMBERS];    /* 是否可以选择该植物 */
	static ui::ScrollView* plantCardTextScrollView;   /* 植物卡牌滚动视图 */
	bool _selectFinished;                             /* 是否选择完成 */
	vector<UserSelectCard> seedBankButton;

protected:
	Global* _global;                           /* 全局变量单例 */
	Sprite* _seedChooser;                      /* 植物选择对话框 */
	Plants* _plant;                            /* 植物变量 */
	SkeletonAnimation* _plantsAnimation;       /* 植物动画 */
	ui::ScrollView* _plantCardScrollView;      /* 植物卡牌滚动视图 */
	float _plantCardScrollViewPercent;         /* 植物卡牌滚动视图滚动距离 */
	float _plantCardRollingDistance;           /* 计算鼠标滚动距离 */
	static EventListenerMouse* _listener;      /* 鼠标监听 */
	static int _listenerMutex;                 /* 鼠标监听互斥 */
	map<PlantsType, Button*>_plantsCards;      /* 植物卡牌 */
};