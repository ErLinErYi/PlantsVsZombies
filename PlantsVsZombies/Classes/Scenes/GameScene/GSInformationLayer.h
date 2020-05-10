/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/GlobalVariable.h"
#include "Based/LevelData.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class GSInformationLayer :public Layer
{
public:
	CREATE_FUNC(GSInformationLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }
	void updateZombiesDieNumbers();
	void updateCoinNumbers();
	void updateSunNumbers();
	void updateProgressBar(const int& zombiesAppearFrequency);
	void updateProgressBar(const int& zombiesAppearFrequency,const int& maxFrequency);
	bool updateProgressBarFlag();
	bool updateProgressBarFlag(const int& id);
	void updateProgressBarFinalFlag();
	void updateProgressBarHead();
	void sunNumberTextWarning();
	void createPromptText();
	void setProgressBarPercent(const float precent);
	float getProgressBarPercent() const;
	void setProgressBarLastPercent(const float precent);
	float getProgressBarLastPercent() const;

CC_CONSTRUCTOR_ACCESS:
	GSInformationLayer();
	~GSInformationLayer();
	virtual bool init();

private:
	void showUserText();
	void showProgressBar();
	void showZombiesDieNumbers();
	void showCoinNumbers();
	void showSunNumbers();
	void showPromptMuchZombiesText(const string& textName);
	
private:
	float _levelLastPrecent;            // 进度条上一次进度
	Text* _zombiesDieText;              // 僵尸死亡数文本
	Text* _coinNumbersText;             // 金币数文本
	Text* _sunNumbersText;              // 阳光数文本
	ProgressTimer* _progressBar;        // 进度条
	Global* _global;                    // 全局变量
	OpenLevelData* _openLevelData;
};