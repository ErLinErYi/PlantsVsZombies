/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/LZGlobalVariable.h"
#include "Based/LZLevelData.h"
#include "Based/LZGameType.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class GSInformationLayer :public Layer
{
public:
	CREATE_FUNC(GSInformationLayer);
	virtual void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }
	virtual void updateZombiesDieNumbers();
	virtual void updateCoinNumbers();
	virtual void updateSunNumbers();
	virtual void updateProgressBar(const int& zombiesAppearFrequency);
	virtual void updateProgressBar(const int& zombiesAppearFrequency,const int& maxFrequency);
	virtual bool updateProgressBarFlag();
	virtual bool updateProgressBarFlag(const int& id);
	virtual void updateProgressBarFinalFlag();
	virtual void updateProgressBarHead();
	virtual void sunNumberTextWarning();
	virtual void createPromptText();
	virtual void setProgressBarPercent(const float precent);
	virtual float getProgressBarPercent() const;
	virtual void setProgressBarLastPercent(const float precent);
	virtual float getProgressBarLastPercent() const;

CC_CONSTRUCTOR_ACCESS:
	GSInformationLayer();
	~GSInformationLayer();
	virtual bool init();

protected:
	virtual void showUserText();
	virtual void showProgressBar();
	virtual void showZombiesDieNumbers();
	virtual void showCoinNumbers();
	virtual void showSunNumbers();
	virtual void showGameType();
	virtual void showPromptMuchZombiesText(const string& textName);
	
public:
	GameType* gameType;

protected:
	float _levelLastPrecent;            // 进度条上一次进度
	Text* _zombiesDieText;              // 僵尸死亡数文本
	Text* _coinNumbersText;             // 金币数文本
	Text* _sunNumbersText;              // 阳光数文本
	ProgressTimer* _progressBar;        // 进度条
	Global* _global;                    // 全局变量
	OpenLevelData* _openLevelData;
};