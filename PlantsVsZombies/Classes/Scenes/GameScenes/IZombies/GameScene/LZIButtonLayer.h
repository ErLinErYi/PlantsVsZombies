/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.02
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSButtonLayer.h"
#include "Zombies/LZZombies.h"

class IButtonLayer :public GSButtonLayer
{
public:
	CREATE_FUNC(IButtonLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	IButtonLayer();
	~IButtonLayer();
	virtual bool init() override;

protected:
	virtual Button* createButton(const string& normal, const string& select, const Vec2& position, GSLayerButton buttonName, const bool isFlippedX = false) override;
	virtual void createQuitDialog() override;
	virtual void createKeyBoardListener() override;
	virtual void createZombiesCard();
	virtual void showZombiesInformation(Button* button, int type);;

public:
	struct ZombiesCardButton
	{
		Button* button;                     // 按钮
		string information[2];              // 文字信息
		unsigned int quality;               // 品质
		unsigned int needSunNumber;         // 需要阳光数
		unsigned int tag;                   // 序号
		ZombiesType type;                   // 僵尸类型
	};
	static vector<ZombiesCardButton> zombiesCardButton;
	static Button* stopButton;

private:
	unsigned int _levelNumber;
};