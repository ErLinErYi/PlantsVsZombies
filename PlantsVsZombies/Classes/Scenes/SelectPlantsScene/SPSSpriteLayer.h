/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/GlobalVariable.h"
#include "spine/spine-cocos2dx.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace spine;

class SPSSpriteLayer : public Layer
{
public:
	CREATE_FUNC(SPSSpriteLayer);
	Text* showPlantsInformation(Button* button, const unsigned int id) const;

private:
	void createSelectPlantsDialog();
	void alreadySelectPlantsDialog();
	void alreadyHavePlantsDialog();
	void createScrollview();
	void scrollViewUpdate(float Time);
	void createMouseListener();
	void createPlantsCards();
	void showRandPlantsInformation();
	Button* createButtons(const Vec2& vec2, const unsigned int& ID);
	void preLoadText();
	void createPlantsImage(Button* button, const std::string& resource, const float& scale = 0.8f) const;
	void createMoveButton(Button* button, const Vec2& vec2, const unsigned int& id);
	void createAnimationAndText(const unsigned int& id);
	void createPlantsAnimation(const std::string& filepath, const std::string& AnimationName, const std::string& skin, Vec2& vec2, const float& scale = 1.0f);
	void createPlantsText(const unsigned int& ID, const std::string& name, const Vec2& vec2, const float& fontsize, Color3B color = Color3B::WHITE, bool AnchorPoint = true);
	void sortPlantsCard(const unsigned int& id);
	void removePlantsCardCallBack(Node* node, const unsigned int& ID, Button* button);
	void createBeginButton();
	void controlPlantCanSelect(Button* button, const unsigned int id);
	void createProhibit(Button* button);
	void selectPlantsCallBack(Node* node) { _selectFinished = true; }

CC_CONSTRUCTOR_ACCESS:
	SPSSpriteLayer();
	~SPSSpriteLayer();
	virtual bool init();

public:
	bool _selectFinished;                      /* 是否选择完成 */
	vector<UserSelectCard> seedBankButton;

private:
	Global* _global;
	Sprite* _seedChooser;
	Text* _animationText[15];                  /* 动画说明 */
	SkeletonAnimation* _plantsAnimation;       /* 植物动画 */
	ui::ScrollView* _plantCardScrollView;      /* 植物卡牌滚动视图 */
	ui::ScrollView* _plantCardTextScrollView;  /* 植物卡牌滚动视图 */
	float _plantCardScrollViewPercent;         /* 植物卡牌滚动视图滚动距离 */
	float _plantCardRollingDistance;           /* 计算鼠标滚动距离 */
	unsigned int _plantsNumber;                /* 植物数量 */
	bool _isPlantIsCanSelect[13];              /* 是否可以选择该植物 */
};

