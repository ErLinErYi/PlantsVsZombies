/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.08
 *Email: 2117610943@qq.com
 */

#include "LZScaredyShroom.h"
#include "Bullet/LZScaredyShroomBullet.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"


ScaredyShroom::ScaredyShroom(Node* node):
	_isHide(false),
	_isHideFinished(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::ScaredyShroom;
}

ScaredyShroom::~ScaredyShroom()
{
}

ScaredyShroom* ScaredyShroom::create(Node* node)
{
	ScaredyShroom* scaredyShroom = new (std::nothrow) ScaredyShroom(node);
	if (scaredyShroom && scaredyShroom->init())
	{
		scaredyShroom->autorelease();
		return scaredyShroom;
	}
	CC_SAFE_DELETE(scaredyShroom);
	return nullptr;
}

Sprite* ScaredyShroom::createPlantImage()
{
	imageInit("ScaredyShroom", INIT);
	_plantImage->setScale(1.23f);
	_plantImage->setAnchorPoint(Vec2(0.4f, 0.5f));
	return _plantImage;
}

void ScaredyShroom::createPlantAnimation()
{
	_plantAnimation = plantInit("ScaredyShroom", "ScaredyShroom_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.45f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.9f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(-10, 12));

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);

	// 创建监听
	createListener("ScaredyShroom_Attack", "attack", "puff");
}

void ScaredyShroom::determineRelativePositionPlantsAndZombies()
{
	checkPlantNeedHide();

	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */

		plantAttack(zombie);         /* 植物攻击 */
	}

	if (!_isHide && !_isHideFinished)
	{
		plantRecovery("ScaredyShroom_Normal");
	}
}

void ScaredyShroom::checkPlantNeedHide()
{
	_isHide = false; //初始不隐藏
	for (auto zombie : ZombiesGroup)
	{
		if (!getZombieIsInPlantAround(zombie) && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap())
		{
			_isHide = true;
			break;
		}
	}

	if (!_isHide && _isHideFinished)
	{
		_isHideFinished = false;
		_plantAnimation->addAnimation(0, "ScaredyShroom_Show", false);
		_plantAnimation->addAnimation(0, "ScaredyShroom_Normal", true);
	}
}

void ScaredyShroom::plantAttack(Zombies* zombie)
{
	if (!_isHide) // 不需要隐藏
	{
		if (_isHideFinished) //如果隐藏完成了，需要恢复
		{
			_isHideFinished = false;
			_plantAnimation->addAnimation(0, "ScaredyShroom_Show", false);
			_plantAnimation->addAnimation(0, "ScaredyShroom_Normal", true);
		}

		if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* 植物存活 && 僵尸没有死亡 && 僵尸进入地图 */
			getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie) &&               /* 僵尸与植物在同一行 && 僵尸在植物的前方 */
			zombie->getZombieIsCanBeAttack())                                                           /* 僵尸是否可以被攻击到 */
		{
			plantEmission("ScaredyShroom_Attack");
		}
	}
	else
	{
		if (!_isHideFinished) //如果没有隐藏完成
		{
			_isHideFinished = true;
			_plantAnimation->addAnimation(0, "ScaredyShroom_Hide", false);
			_plantAnimation->addAnimation(0, "ScaredyShroom_Hide_Normal", true);
		}
	}
}

bool ScaredyShroom::getZombieIsInPlantAround(Zombies* zombie)
{
	if (fabs(zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX()) < 200 &&
		fabs((zombie->getZombieAnimation()->getPositionY() + 50) - (_plantAnimation->getPositionY() + 60)) < 200)
	{
		return false;
	}
	return true;
}

void ScaredyShroom::plantEmission(const string& plantAnimation)
{
	_isHaveZombies = true; /* 表示有僵尸与植物在同一行 */
	if (!_isChanged)     /* 判断动画是否已经改变 */
	{
		_plantAnimation->addAnimation(0, plantAnimation, true);
		_isChanged = true;
	}
}

void ScaredyShroom::plantRecovery(const string& plantAnimation)
{
	if (!_isHaveZombies) /* 判断是否有僵尸与在同一行 */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, plantAnimation, true);
			_isChanged = false;
		}
	}

	_isHaveZombies = false; /* 每循环一次就初始化 */
}

void ScaredyShroom::createBullet()
{
	_bulletAnimation = new ScaredyShroomBullet(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	_bulletAnimation->createBullet();

	BulletGroup.emplace_back(_bulletAnimation);
}

SkeletonAnimation* ScaredyShroom::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("SCAREDYSHROOM_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("ScaredyShroom", "ScaredyShroom_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.8f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("SCAREDYSHROOM_1")->second->text, Vec2(190, 910), lta.find("SCAREDYSHROOM_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("SCAREDYSHROOM_2")->second->text, Vec2(360, 1000), lta.find("SCAREDYSHROOM_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("SCAREDYSHROOM_3")->second->text, Vec2(440, 1000), lta.find("SCAREDYSHROOM_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::ScaredyShroom, "SCAREDYSHROOM_4", "SCAREDYSHROOM_5"), Vec2(360, 870),
		lta.find("SCAREDYSHROOM_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::ScaredyShroom)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}
