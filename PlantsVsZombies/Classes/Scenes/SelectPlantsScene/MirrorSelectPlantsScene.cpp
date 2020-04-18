/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.17
 *Email: 2117610943@qq.com
 */

#include "MirrorSelectPlantsScene.h"
#include "../GameScene/MirrorGameScene.h"
#include "SPSSpriteLayer.h"

Scene* MirrorSelectPlantsScene::createScene()
{
    return MirrorSelectPlantsScene::create();
}

void MirrorSelectPlantsScene::readyTextCallBack(Node* node, const std::string& name, const int& id)
{
	this->removeChildByName(name);
	switch (id)
	{
	case 1:
		createReadyText("StartSet", 2);
		break;
	case 2:
		createReadyText("StartPlant", 3);
		break;
	default:
		_scrollLayer->removeChildByName("previewBackgroundImage");

		_global->userInformation->setUserSelectCrads(_spriteLayer->seedBankButton);
		_global->userInformation->setSunNumbers(100); //设定初始阳光数 

		Director::getInstance()->replaceScene(MirrorGameScene::createScene());
		break;
	}
}
