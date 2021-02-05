/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.31
 *Emal: 2117610943@qq.com
 */

#pragma once
#include "Based/LZBDialog.h"

#include "spine/spine.h"
#include "spine/spine-cocos2dx.h"

using namespace spine;

class Plants;
enum class PlantsType;

class PlantsCardUnlock :public Dialog
{
CC_CONSTRUCTOR_ACCESS:
	CREATE_FUNC(PlantsCardUnlock);
	PlantsCardUnlock();
	~PlantsCardUnlock();
	virtual bool init();

public:
	static vector<PlantsType> unlockPlantsCard;

private:
	void createDialog();
	void createButtons();
	void showUnlockPlant();
	void showNextUnlockPlant();
	void showDialogAction();
	void fadeOutAndShowNewDialog();
	
private:
	Sprite* _plantsCardUnlockDialog;
	ui::ScrollView* _scrollView;
	Plants* _plant;
	SkeletonAnimation* _plantAnimation;
	Size _winSize;
};