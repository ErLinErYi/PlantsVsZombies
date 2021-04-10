/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#include "LZTGGameOverLayer.h"
#include "Scenes/MainMenuScene/LZMainMenu.h"
#include "Based/LZPlayMusic.h"

TGGameOverLayer::TGGameOverLayer()
{
}

TGGameOverLayer::~TGGameOverLayer()
{
}

bool TGGameOverLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	setQuitGame();

	return true;
}

void TGGameOverLayer::setQuitGame()
{
	PlayMusic::playMusic("losemusic");

	auto brain = Sprite::createWithSpriteFrameName("Brain.png");
	brain->setPosition(_director->getWinSize() / 2.0f);
	brain->setScale(2.0f);
	brain->setGlobalZOrder(1);
	brain->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.05f, Vec2(10, 0)), MoveBy::create(0.05f, Vec2(-10, 0)), nullptr)));
	this->addChild(brain);

	auto lose = Sprite::createWithSpriteFrameName(rand() % 2 == 0 ? "zombieswon.png" : "zombieswon1.png");
	lose->setScale(2.0f);
	lose->setGlobalZOrder(1);
	lose->setPosition(Vec2(2500, Director::getInstance()->getWinSize().height / 2.0f));
	lose->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.05f, Vec2(10, 0)), MoveBy::create(0.05f, Vec2(-10, 0)), nullptr)));
	lose->runAction(Sequence::create(JumpTo::create(1.0f, Director::getInstance()->getWinSize() / 2.0f, 250, 3),
		DelayTime::create(5), CallFunc::create([this]() {Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MainMenu::create())); }), nullptr));
	this->addChild(lose);
}
