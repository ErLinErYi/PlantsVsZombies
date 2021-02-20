/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZPeaShooter.h"
#include "Bullet/LZPea.h"

#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"


PeaShooter::PeaShooter(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::PeaShooter;
}

PeaShooter::~PeaShooter()
{
}

PeaShooter* PeaShooter::create(Node* node)
{
	PeaShooter* peaShooter = new (std::nothrow) PeaShooter(node);
	if (peaShooter && peaShooter->init())
	{
		peaShooter->autorelease();
		return peaShooter;
	}
	CC_SAFE_DELETE(peaShooter);
	return nullptr;
}

Sprite* PeaShooter::createPlantImage()
{
	imageInit("PeaShooter", INIT);
	_plantImage->setScale(1.3f);
	_plantImage->setAnchorPoint(Vec2(0.35f, 0.6f));
	return _plantImage;
}

void PeaShooter::createPlantAnimation()
{
	_plantAnimation = plantInit("PeaShooter", "PeaShooter_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.0f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.9f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 15));

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	// 创建监听
	createListener("PeaShooter_Shoot");
}

void PeaShooter::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */

		plantAttack(zombie);         /* 植物攻击 */
	}

	plantRecovery("PeaShooter_Normal");
}

void PeaShooter::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* 植物存活 && 僵尸没有死亡 && 僵尸进入地图 */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* 僵尸与植物在同一行 && 僵尸在植物的前方 */
	{
		plantEmission("PeaShooter_Shoot");
	}
}

void PeaShooter::plantEmission(const string& plantAnimation)
{
	_isHaveZombies = true; /* 表示有僵尸与植物在同一行 */
	if (!_isChanged)     /* 判断动画是否已经改变 */
	{
		_plantAnimation->addAnimation(0, plantAnimation, true);
		_isChanged = true;
	}
}

void PeaShooter::plantRecovery(const string& plantAnimation)
{
	if (!_isHaveZombies) /* 判断是否有僵尸与豌豆射手在同一行 */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, plantAnimation, true);
			_isChanged = false;
		}
	}

	_isHaveZombies = false; /* 每循环一次就初始化 */
}

void PeaShooter::createBullet()
{
	_bulletAnimation= new Pea(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}

SkeletonAnimation* PeaShooter::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("PEASHOOTER_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("PeaShooter", "PeaShooter_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.8f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("PEASHOOTER_1")->second->text, Vec2(190, 910), lta.find("PEASHOOTER_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("PEASHOOTER_2")->second->text, Vec2(360, 1000), lta.find("PEASHOOTER_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("PEASHOOTER_3")->second->text, Vec2(440, 1000), lta.find("PEASHOOTER_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, lta.find("PEASHOOTER_4")->second->text, Vec2(360, 830), lta.find("PEASHOOTER_4")->second->fontsize, Color3B::ORANGE, false);

	return _plantAnimation;
}