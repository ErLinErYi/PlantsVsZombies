/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"

class HInformationLayer :public GSInformationLayer
{
public:
	CREATE_FUNC(HInformationLayer);
	virtual void showHammerInformation(Button* button);
	virtual void deleteHammerInformation();

CC_CONSTRUCTOR_ACCESS:
	HInformationLayer();
	~HInformationLayer();
	virtual bool init();

protected:
	virtual void showUserText() override;
	virtual void showProgressBar() override;
	virtual void showTipsText();
	virtual void setBeginAnimation();

private:
	Layer* _informationLayer;
	Text* _hammerText;
};