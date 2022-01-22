/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"

class TGInformationLayer :public GSInformationLayer
{
public:
	CREATE_FUNC(TGInformationLayer);
	virtual void addLayer(Node* node, const int order, const string& name) override { node->addChild(this, order, name); }
	virtual void updateText();

CC_CONSTRUCTOR_ACCESS:
	TGInformationLayer();
	~TGInformationLayer();
	virtual bool init() override;

protected:
	virtual void showUserText() override;
	virtual void showSunNumbers() override;
	virtual void showGameType() override;
	virtual void onSetMostFrenquence(int frequence);
	virtual int onGetMostFrenquence();

private:
	string _text;
	Text* _userName;
	int _mostFrenquence;
	int _encryptKey;
};
