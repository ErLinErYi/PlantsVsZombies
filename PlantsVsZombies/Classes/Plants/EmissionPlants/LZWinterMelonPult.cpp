/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.3
 *Emal: 2117610943@qq.com
 */

#include "LZWinterMelonPult.h"
#include "Bullet/LZWinterMelonBullet.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

WinterMelonPult::WinterMelonPult(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_plantsType = PlantsType::WinterMelonPult;

	_zombiePostion = Vec2::ZERO;
	_zombie = nullptr;
}

WinterMelonPult::~WinterMelonPult()
{
}

WinterMelonPult* WinterMelonPult::create(Node* node)
{
	WinterMelonPult* winterMelonPult = new (std::nothrow) WinterMelonPult(node);
	if (winterMelonPult && winterMelonPult->init())
	{
		winterMelonPult->autorelease();
		return winterMelonPult;
	}
	CC_SAFE_DELETE(winterMelonPult);
	return nullptr;
}

Sprite* WinterMelonPult::createPlantImage()
{
	imageInit("WinterMelonPult", INIT);
	_plantImage->setScale(1.8f);
	_plantImage->setAnchorPoint(Vec2(0.7f, 0.45f));
	return _plantImage;
}

void WinterMelonPult::createPlantAnimation()
{
	_plantAnimation = plantInit("WinterMelonPult", "WinterMelonPult_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setPosition(_position + Vec2(10, 0));
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(2.2f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.0f);

	// 创建监听
	createListener("WinterMelonPult_Shoot", "shoot");
}

void WinterMelonPult::determineRelativePositionPlantsAndZombies()
{
	CabbagePult::determineRelativePositionPlantsAndZombies("WinterMelonPult_Normal");
}

void WinterMelonPult::plantAttack(Zombies* zombie)
{
	CabbagePult::plantAttack(zombie, "WinterMelonPult_Shoot");
}

void WinterMelonPult::createBullet()
{
	_bulletAnimation = new WinterMelonBullet(_node);
	_bulletAnimation->setBulletPosition(_position + Vec2(0, 20));
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombiePosition(_zombiePostion);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombieSpeed(_zombieSpeed);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombieHeight(_zombieHeight);
	_bulletAnimation->createBullet();

	BulletGroup.emplace_back(_bulletAnimation);
}

SkeletonAnimation* WinterMelonPult::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("WINTERMELONPULT_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("WinterMelonPult", "WinterMelonPult_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.5f);
	_plantAnimation->setPosition(Vec2(250, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("WINTERMELONPULT_1")->second->text, Vec2(190, 910), lta.find("WINTERMELONPULT_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("WINTERMELONPULT_2")->second->text, Vec2(360, 1000), lta.find("WINTERMELONPULT_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("WINTERMELONPULT_3")->second->text, Vec2(440, 1000), lta.find("WINTERMELONPULT_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::WinterMelonPult, "WINTERMELONPULT_4", "WINTERMELONPULT_5"), Vec2(360, 870),
		lta.find("WINTERMELONPULT_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::WinterMelonPult)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}
