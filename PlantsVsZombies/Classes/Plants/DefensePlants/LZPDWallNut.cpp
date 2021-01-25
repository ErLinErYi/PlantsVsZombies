/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZPDWallNut.h"
#include "Scenes/SelectPlantsScene/LZSSSpriteLayer.h"

WallNut::WallNut(Node* node):
	_skinId(1)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 4000;
	_totalHealthPoint = 4000;
	_plantsType = PlantsType::WallNut;
}

WallNut::~WallNut()
{
}

WallNut* WallNut::create(Node* node)
{
	WallNut* wallNut = new (std::nothrow) WallNut(node);
	if (wallNut && wallNut->init())
	{
		wallNut->autorelease();
		return wallNut;
	}
	CC_SAFE_DELETE(wallNut);
	return nullptr;
}
Sprite* WallNut::createPlantImage()
{
	imageInit("Wallnut_body", INIT);
	_plantImage->setScale(1.5f);
	return _plantImage;
}

void WallNut::createPlantAnimation()
{
	_plantAnimation = plantInit("WallNut", "WallNut_Nornal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.8f);
	_plantAnimation->setSkin("WallNut_Normal");
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(2.2f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.0f);
}

void WallNut::checkPlantHealthPoint()
{
	if (_healthPoint <= _totalHealthPoint * 2.f / 3.f && _skinId == 1)
	{
		_plantAnimation->setSkin("WallNut_Cracked1");
		_skinId = 2;
	}
	if (_healthPoint <= _totalHealthPoint / 3.f && _skinId == 2)
	{
		_plantAnimation->setSkin("WallNut_Cracked2");
		_skinId = 3;
	}
}

SkeletonAnimation* WallNut::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("WALLNUT_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("WallNut", "WallNut_Nornal");
	_plantAnimation->setSkin("WallNut_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.5f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("WALLNUT_1")->second->text, Vec2(190, 910), lta.find("WALLNUT_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("WALLNUT_2")->second->text, Vec2(360, 1000), lta.find("WALLNUT_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("WALLNUT_3")->second->text, Vec2(440, 1000), lta.find("WALLNUT_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, lta.find("WALLNUT_4")->second->text, Vec2(360, 870), lta.find("WALLNUT_4")->second->fontsize, Color3B::YELLOW, false);
    
	return _plantAnimation;
}
