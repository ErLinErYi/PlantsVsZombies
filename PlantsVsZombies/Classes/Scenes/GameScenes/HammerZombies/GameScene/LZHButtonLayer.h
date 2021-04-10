/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSButtonLayer.h"

class HButtonLayer :public GSButtonLayer
{
public:
	CREATE_FUNC(HButtonLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	HButtonLayer();
	~HButtonLayer();
	virtual bool init();

protected:
	virtual Button* createButton(const string& normal, const string& select, const Vec2& position, GSLayerButton buttonName, const bool isFlippedX = false) override;
	virtual void createQuitDialog() override;
	virtual void createKeyBoardListener() override;
	virtual void createHammerButton();
	virtual void loadButtonInforamtion(Button* button);
	virtual void updateHammerButton();

public:
	static Button* stopButton;
	static unsigned int selectedHammer;
	static vector<Button*> hammerButton;
	struct HammerInformation
	{
		string information;                 // ������Ϣ
		unsigned int quality;               // Ʒ��
		unsigned int lowAttack;
		unsigned int HigtAttack;            // ������
		unsigned int unLock;                // ����
	};

private:
	unsigned int _levelNumber;
};

extern HButtonLayer::HammerInformation hammerInformation[7];