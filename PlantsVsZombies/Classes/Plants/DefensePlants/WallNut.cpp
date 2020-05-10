/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "WallNut.h"

WallNut::WallNut(Node* node):
	_skinId(1)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 3000;
	_totalHealthPoint = 3000;
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