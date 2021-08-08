/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.02
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"

class IInformationLayer :public GSInformationLayer
{
public:
	CREATE_FUNC(IInformationLayer);

CC_CONSTRUCTOR_ACCESS:
	IInformationLayer();
	~IInformationLayer();
	virtual bool init() override;

protected:
	virtual void showUserText() override;
	virtual void showSunNumbers() override;

private:
	Layer* _informationLayer;
};