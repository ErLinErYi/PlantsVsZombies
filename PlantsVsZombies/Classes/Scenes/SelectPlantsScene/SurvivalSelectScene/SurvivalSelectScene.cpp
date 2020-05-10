/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.5.08
 *Email: 2117610943@qq.com
 */

#include "SurvivalSelectScene.h"
#include "../SPSSpriteLayer.h"
#include "../SPSBackgroundLayer.h"

#include "Scenes/GameScene/SurvivalGameScene/SurvivalGameScene.h"

Scene* SurvivalSelectScene::createScene()
{
    return SurvivalSelectScene::create();
}

bool SurvivalSelectScene::init()
{
    if (!Scene::init())return false;

    createBackgroundLayer();
    createSelectPlantsDialog();
 
    return true;
}

void SurvivalSelectScene::createBackgroundLayer()
{
	this->addChild(SPSBackgroundLayer::create());

	_global->userInformation->getBackground()->setPosition(Vec2(-1010, 0));
}

void SurvivalSelectScene::createSelectPlantsDialog()
{
    _spriteLayer = SPSSpriteLayer::create();
    this->addChild(_spriteLayer);

    schedule(schedule_selector(SurvivalSelectScene::eventUpdate), 0.22f);
}

void SurvivalSelectScene::eventUpdate(float time)
{
    if (_spriteLayer->_selectFinished)
    {
        unschedule(schedule_selector(SurvivalSelectScene::eventUpdate));
       
        replaceScene();
    }
}

void SurvivalSelectScene::replaceScene()
{
    _global->userInformation->setUserSelectCrads(_spriteLayer->seedBankButton);

    Director::getInstance()->replaceScene(SurvivalGameScene::createScene());
}
