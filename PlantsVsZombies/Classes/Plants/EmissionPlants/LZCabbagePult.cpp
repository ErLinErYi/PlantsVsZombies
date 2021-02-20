/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Emal: 2117610943@qq.com
 */

#include "LZCabbagePult.h"
#include "LZPeaShooter.h"
#include "Bullet/LZCabbage.h"

#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

CabbagePult::CabbagePult(Node* node):
    _zombiePostion(Vec2::ZERO)
,   _zombie(nullptr)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0; // !!!暂时没有使用 使用的是cabbage里面的_attack
	_plantsType = PlantsType::CabbagePult;
}

CabbagePult::~CabbagePult()
{
}

CabbagePult* CabbagePult::create(Node* node)
{
	CabbagePult* cabbagePult = new (std::nothrow) CabbagePult(node);
	if (cabbagePult && cabbagePult->init())
	{
		cabbagePult->autorelease();
		return cabbagePult;
	}
	CC_SAFE_DELETE(cabbagePult);
	return nullptr;
}
Sprite* CabbagePult::createPlantImage()
{
	imageInit("Cabbage", INIT);
	_plantImage->setScale(1.5f);
	_plantImage->setAnchorPoint(Vec2(0.6f, 0.6f));
	return _plantImage;
}

void CabbagePult::createPlantAnimation()
{
	_plantAnimation = plantInit("Cabbage", "Cabbage_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setTimeScale(1.f);
	_plantAnimation->setScale(0.8f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(2.6f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.0f);

	// 创建监听
	createListener("Cabbage_Shoot", "Shoot");
}

void CabbagePult::determineRelativePositionPlantsAndZombies()
{
	determineRelativePositionPlantsAndZombies("Cabbage_Normal");
}

void CabbagePult::plantAttack(Zombies* zombie)
{
	plantAttack(zombie, "Cabbage_Shoot");
}

void CabbagePult::determineRelativePositionPlantsAndZombies(const string& animationName)
{
	_distance = _MAX_; /* 初始化距离变量 */

	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		plantAttack(zombie);         /* 植物攻击 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}

	if (_isHaveZombies)  /* 如果有僵尸与卷心菜投手在同一行 */
	{
		_zombiePostion = _zombie->getZombieAnimation()->getPosition();
		_zombieSpeed = _zombie->getZombieCurrentSpeed();
		//_zombieHeight = _zombie->getZombieAnimation()->getScale() * _zombie->getZombieAnimation()->getBoundingBox().size.height;
	}

	if (!_isHaveZombies) /* 如果没有僵尸与卷心菜投手在同一行 */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, animationName, true);
			_isChanged = false;
		}
	}

	_isHaveZombies = false; /* 每循环一次就初始化 */
}

void CabbagePult::plantAttack(Zombies* zombie, const string& animationName)
{
	if (zombie->getZombieIsSurvive() && getPlantIsSurvive() && zombie->getZombieIsEnterMap() && /* 僵尸存活 && 植物存活 && 僵尸进入地图 */
		getZombieIsSameLineWithPlant(zombie) && getZombieIsTheFrontOfPlant(zombie))             /* 与植物在同一行 && 僵尸在植物的前方 */
	{
		_isHaveZombies = true; /* 表示有僵尸与植物在同一行 */
		if (!_isChanged)
		{
			_plantAnimation->addAnimation(0, animationName, true);
			_isChanged = true;
		}

		float distance = zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX();
		if (_distance > distance) /* 僵尸与植物的距离小于上一个僵尸与植物的距离 */
		{
			_distance = distance;
			_zombie = zombie;     /* 记录该僵尸 */
		}
	}
}

void CabbagePult::createBullet()
{
	_bulletAnimation = new Cabbage(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombiePosition(_zombiePostion);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombieSpeed(_zombieSpeed);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombieHeight(_zombieHeight);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}

SkeletonAnimation* CabbagePult::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("CABBAGE_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Cabbage", "Cabbage_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.3f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("CABBAGE_1")->second->text, Vec2(190, 910), lta.find("CABBAGE_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("CABBAGE_2")->second->text, Vec2(360, 1000), lta.find("CABBAGE_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("CABBAGE_3")->second->text, Vec2(440, 1000), lta.find("CABBAGE_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, lta.find("CABBAGE_4")->second->text, Vec2(360, 870), lta.find("CABBAGE_4")->second->fontsize, Color3B::ORANGE, false);

	return _plantAnimation;
}