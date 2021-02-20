/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.29
 *Email: 2117610943@qq.com
 */

#include "LZThreePeaShooter.h"
#include "Bullet/LZPea.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

ThreePeaShooter::ThreePeaShooter(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::ThreePeaShooter;
}

ThreePeaShooter::~ThreePeaShooter()
{
}

ThreePeaShooter* ThreePeaShooter::create(Node* node)
{
	ThreePeaShooter* threePeaShooter = new (std::nothrow) ThreePeaShooter(node);
	if (threePeaShooter && threePeaShooter->init())
	{
		threePeaShooter->autorelease();
		return threePeaShooter;
	}
	CC_SAFE_DELETE(threePeaShooter);
	return nullptr;
}

Sprite* ThreePeaShooter::createPlantImage()
{
	imageInit("ThreePeaShooter", INIT);
	_plantImage->setScale(1.5f);
	_plantImage->setAnchorPoint(Vec2(0.4f, 0.6f));
	return _plantImage;
}

void ThreePeaShooter::createPlantAnimation()
{
	_plantAnimation = plantInit("ThreePeaShooter", "ThreePeaShooter_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.0f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.9f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 5));

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	// 创建监听
	createListener("ThreePeaShooter_Shoot");
}

void ThreePeaShooter::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */

		plantAttack(zombie);         /* 植物攻击 */
	}

	plantRecovery("ThreePeaShooter_Normal");
}

void ThreePeaShooter::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* 植物存活 && 僵尸没有死亡 && 僵尸进入地图 */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* 僵尸在植物的前方 && 僵尸与植物在同一行 */
	{
		plantEmission("ThreePeaShooter_Shoot");
	}
}

bool ThreePeaShooter::getZombieIsSameLineWithPlant(Zombies* zombie)
{
	return fabs(zombie->getZombieInRow() - getPlantRow()) <= 1;
}

void ThreePeaShooter::createBullet()
{
	for (int i = 0; i < 3; ++i)
	{
		_bulletAnimation = new Pea(_node);
		_bulletAnimation->setBulletPosition(_position - Vec2(30, 0));
		_bulletAnimation->setBulletInRow(_rowAndColumn.y + 1 - i);
		dynamic_cast<Pea*>(_bulletAnimation)->setPeaDirectionType(static_cast<Pea::PeaDirectionType>(i));
		_bulletAnimation->createBullet();
		_bulletAnimation->getBullet()->setScale(0.9f);

		BulletGroup.push_back(_bulletAnimation);
	}
}

SkeletonAnimation* ThreePeaShooter::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("THREEPEASHOOTER_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("ThreePeaShooter", "ThreePeaShooter_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.6f);
	_plantAnimation->setPosition(Vec2(180, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("THREEPEASHOOTER_1")->second->text, Vec2(190, 910), lta.find("THREEPEASHOOTER_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("THREEPEASHOOTER_2")->second->text, Vec2(360, 1000), lta.find("THREEPEASHOOTER_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("THREEPEASHOOTER_3")->second->text, Vec2(440, 1000), lta.find("THREEPEASHOOTER_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::ThreePeaShooter, "THREEPEASHOOTER_4", "THREEPEASHOOTER_5"), Vec2(360, 870),
		lta.find("THREEPEASHOOTER_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::ThreePeaShooter)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}