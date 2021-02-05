/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.3
 *Emal: 2117610943@qq.com
 */

#include "LZPEWaterMelonPult.h"
#include "Bullet/LZPEBWaterMelonBullet.h"
#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"

WaterMelonPult::WaterMelonPult(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_plantsType = PlantsType::WaterMelonPult;

	_zombiePostion = Vec2::ZERO;
	_zombie = nullptr;
}

WaterMelonPult::~WaterMelonPult()
{
}

WaterMelonPult* WaterMelonPult::create(Node* node)
{
	WaterMelonPult* waterMelonPult = new (std::nothrow) WaterMelonPult(node);
	if (waterMelonPult && waterMelonPult->init())
	{
		waterMelonPult->autorelease();
		return waterMelonPult;
	}
	CC_SAFE_DELETE(waterMelonPult);
	return nullptr;
}

Sprite* WaterMelonPult::createPlantImage()
{
	imageInit("WaterMelonPult", INIT);
	_plantImage->setScale(1.8f);
	_plantImage->setAnchorPoint(Vec2(0.7f, 0.45f));
	return _plantImage;
}

void WaterMelonPult::createPlantAnimation()
{
	_plantAnimation = plantInit("WaterMelonPult", "WaterMelonPult_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(2.2f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.0f);

	// 创建监听
	createListener("WaterMelonPult_Shoot");
}

void WaterMelonPult::determineRelativePositionPlantsAndZombies()
{
	CabbagePult::determineRelativePositionPlantsAndZombies("WaterMelonPult_Normal");
}

void WaterMelonPult::plantAttack(Zombies* zombie)
{
	CabbagePult::plantAttack(zombie, "WaterMelonPult_Shoot");
}

void WaterMelonPult::createBullet()
{
	_isCreateBullet = true;

	_bulletAnimation = new WaterMelonBullet(_node);
	_bulletAnimation->setBulletPosition(_position + Vec2(0, 20));
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombiePosition(_zombiePostion);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombieSpeed(_zombieSpeed);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombieHeight(_zombieHeight);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}

SkeletonAnimation* WaterMelonPult::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("WATERMELONPULT_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("WaterMelonPult", "WaterMelonPult_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.5f);
	_plantAnimation->setPosition(Vec2(250, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("WATERMELONPULT_1")->second->text, Vec2(190, 910), lta.find("WATERMELONPULT_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("WATERMELONPULT_2")->second->text, Vec2(360, 1000), lta.find("WATERMELONPULT_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("WATERMELONPULT_3")->second->text, Vec2(440, 1000), lta.find("WATERMELONPULT_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::WaterMelonPult, "WATERMELONPULT_4", "WATERMELONPULT_5"), Vec2(360, 870),
		lta.find("WATERMELONPULT_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::WaterMelonPult)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}
